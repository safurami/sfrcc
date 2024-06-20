/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

#include <filesystem>

lexer::lexer(const char *f, symbol_table* table)
{
  this->m_file.open(f);
  if(!this->m_file.is_open())
  {
    printf("File '%s' was not found\n", f);
    this->m_was_error = true;
    this->m_buffer = nullptr;
    this->m_current_token = nullptr;
    return;
  }
  this->m_table = table;
  this->m_current_line = 1;
  this->m_current_token = new token();

  long unsigned int file_length = std::filesystem::file_size(f);

#ifdef DEBUG
  printf("[Lexer Init] Allocating %ld bytes buffer\n", file_length + 1);
#endif

  this->m_buffer = new char[file_length + 1]();

  this->m_file.read(this->m_buffer, file_length);
  this->m_input = this->m_buffer;
}

lexer::~lexer()
{
  if(this->m_file.is_open())
  {
    this->m_file.close();
  }
  if(this->m_buffer != nullptr)
  {
    delete[] this->m_buffer;
  }
  if(this->m_current_token != nullptr)
  {
    delete this->m_current_token;
  }
}

char lexer::advance()
{
  for(;*this->m_input == '\n'; this->m_current_line++, this->m_input++) {}
  if(*this->m_input == '\0') { return EOF; }
  return *this->m_input++;
}

char lexer::peek()
{
  if(*this->m_input == '\0') return EOF;
  return *this->m_input;
}

bool lexer::match(char sym)
{
  if(this->peek() == sym)
  {
    this->advance();
    return true;
  }
  return false;
}

token* lexer::get_next_token()
{
  int saved_line; // For a few cases.
again:
  char sym = this->advance();
  this->m_lexeme_start = this->m_input-1;
  switch(sym)
  {
  case EOF: this->m_current_token->set_parameters(token_type::DOLLAR, this->m_current_line); break;
  case ' ': goto again; break;
  case '\t': goto again; break;
  case '(': this->m_current_token->set_parameters(token_type::OPEN_PAREN, this->m_current_line); break;
  case ')': this->m_current_token->set_parameters(token_type::CLOSE_PAREN, this->m_current_line); break;
  case '=': this->m_current_token->set_parameters((this->match('=') ? token_type::IS_EQUAL : token_type::ASSIGN), this->m_current_line); break;
  case '!': this->m_current_token->set_parameters((this->match('=') ? token_type::NEQ : token_type::BANG), this->m_current_line); break;
  case '>': this->m_current_token->set_parameters((this->match('=') ? token_type::GEQ : token_type::GREATER), this->m_current_line); break;
  case '<': this->m_current_token->set_parameters((this->match('=') ? token_type::LEQ : token_type::LESS), this->m_current_line); break;
  case ':': this->m_current_token->set_parameters(token_type::COLON, this->m_current_line); break;
  case ';': this->m_current_token->set_parameters(token_type::SEMICOLON, this->m_current_line); break;
  case '{': this->m_current_token->set_parameters(token_type::OPEN_CURLYB, this->m_current_line); break;
  case '}': this->m_current_token->set_parameters(token_type::CLOSE_CURLYB, this->m_current_line); break;
  case ',': this->m_current_token->set_parameters(token_type::COMMA, this->m_current_line); break;
  case '[': this->m_current_token->set_parameters(token_type::SQ_OPENB, this->m_current_line); break;
  case ']': this->m_current_token->set_parameters(token_type::SQ_CLOSEB, this->m_current_line); break;
  case '+': this->m_current_token->set_parameters(token_type::PLUS, this->m_current_line); break;
  case '-': this->m_current_token->set_parameters(token_type::MINUS, this->m_current_line); break;
  case '*': this->m_current_token->set_parameters(token_type::STAR, this->m_current_line); break;
  case '/':
    if(this->match('/'))
    {
      this->skip_line_comment();
      goto again;
    }
    else if(this->match('*'))
    {
      this->skip_multline_comment();
      goto again;
    }
    else
    {
      this->m_current_token->set_parameters(token_type::DIVIDE, this->m_current_line);
    }
    break;
  case '.': this->m_current_token->set_parameters(token_type::DOT, this->m_current_line); break;
  case '&': this->m_current_token->set_parameters(token_type::AND, this->m_current_line); break;
  case '|': this->m_current_token->set_parameters(token_type::OR, this->m_current_line); break;
  case '\'':
    this->m_current_token->set_attribute(this->advance());
    if(!this->match('\''))
    {
      this->report_error(this->peek());
      while(this->advance() != '\'') {}
      goto again;
    }
    this->m_current_token->set_parameters(token_type::CONS, this->m_current_line);
    break;
  case '"':
    saved_line = this->m_current_line;
    if(this->collect_literal())
    {
      report_error(saved_line ,"Nonterminated \".\n");
      goto again;
    }
    break;
  case 'i':
    if(this->check_rest("nt") && !my::isalnum(*this->m_input))
    {
      this->m_current_token->set_parameters(token_type::INT, this->m_current_line);
    }
    else if(this->match('f') && !my::isalnum(*this->m_input))
    {
      this->m_current_token->set_parameters(token_type::IF, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'c':
    if(this->check_rest("har") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::CHAR, this->m_current_line);
    }
    else if(this->check_rest("onst") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::CONST, this->m_current_line);
    }
    else if(this->check_rest("ase") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::CASE, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'v':
    if(this->check_rest("oid") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::VOID, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'u':
    if(this->check_rest("nsigned") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::UNSIGNED, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'f':
    if(this->check_rest("loat") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::FLOAT, this->m_current_line);
    }
    else if(this->check_rest("or") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::FOR, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'd':
    if(this->check_rest("ouble") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::DOUBLE, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 's':
    if(this->check_rest("igned") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::SIGNED, this->m_current_line);
    }
    else if(this->check_rest("hort") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::SHORT, this->m_current_line);
    }
    else if(this->check_rest("witch") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::SWITCH, this->m_current_line);
    }
    else if(this->check_rest("truct") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::STRUCT, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'e':
    if(this->check_rest("lse") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::ELSE, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'r':
    if(this->check_rest("eturn") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::RETURN, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'w':
    if(this->check_rest("hile") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::WHILE, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'b':
    if(this->check_rest("reak") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::BREAK, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  case 'l':
    if(this->check_rest("ong") && !my::isalnum(this->peek()))
    {
      this->m_current_token->set_parameters(token_type::LONG, this->m_current_line);
    }
    else
    {
      this->collect_id();
    }
    break;
  default:
    if(my::isalpha(sym))
    {
      this->collect_id();
    }
    else if(my::isdigit(sym))
    {
      this->collect_number();
    }
    else
    {
      this->report_error(sym);
    }
  }

#ifdef DEBUG
  printf("[Lexer] Returning <Line: %03d, Type: %s, Attribute: %03d>\n", this->m_current_token->get_line(), token_type_to_string(this->m_current_token->get_type()), this->m_current_token->get_attribute());
#endif

  return this->m_current_token;
}

// Return 1 if there is no terminating " character
int lexer::collect_literal()
{
  this->m_input = this->m_lexeme_start + 1;
  for(char c; (c = this->advance()), c != '"';)
  {
    if(c == EOF)
    {
      return 1;
    }
  }

  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_start + 1, this->m_input -2, node_type::LITERAL));
  this->m_current_token->set_parameters(token_type::LITERAL, this->m_current_line);
  return 0;
}

void lexer::collect_id()
{
  this->m_input = this->m_lexeme_start;
  for(;my::isalnum(this->peek()); this->advance()) {}
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_start, this->m_input - 1, node_type::IDENTIFIER));
  this->m_current_token->set_parameters(token_type::IDENTIFIER, this->m_current_line);
}

void lexer::collect_number()
{
  this->m_input = this->m_lexeme_start;
  for(; my::isdigit(*this->m_input); this->advance()) {}
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_start, this->m_input - 1, node_type::NUMBER));
  this->m_current_token->set_parameters(token_type::NUMBER, this->m_current_line);
}

void lexer::report_error(char sym)
{
  printf("[Line %d] Unexpected character '%c'.\n", this->m_current_line, sym);
  this->m_was_error = true;
}

void lexer::report_error(int line, const char *message)
{
  printf("[Line %d] %s\n", line, message);
  this->m_was_error = true;
}

bool lexer::check_rest(const char* rest)
{
  char *save = this->m_input;
  for(int i = 0; rest[i] != '\0'; i++)
  {
    if(this->peek() != rest[i])
    {
      // TODO: recover saved pointer, but advance() could increment line counter.
      this->m_input = save;
      return false;
    }
    this->advance();
  }
  return true;
}

bool lexer::was_error()
{
  return this->m_was_error;
}

void lexer::skip_line_comment()
{
  // TODO: make that after skipping comment, m_input will point to the next character after terminating comments characters.
}

void lexer::skip_multline_comment()
{
  // TODO: make that after skipping comment, m_input will point to the next character after terminating comments characters.
}
