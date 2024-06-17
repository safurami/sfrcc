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
  char *tmp;
  for(tmp = this->m_input; *tmp == '\n'; tmp++) {}

  if(*tmp == '\0') return EOF;
  return *tmp;
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
again:
  int saved_line;
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
      this->report_error();
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
  for(; my::isdigit(this->peek()); this->advance()) {}
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_start, this->m_input - 1, node_type::NUMBER));
  this->m_current_token->set_parameters(token_type::NUMBER, this->m_current_line);
}

void lexer::report_error()
{
  printf("[Line %d] Unexpected character '%c'.\n", this->m_current_line, this->peek());
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
}

void lexer::skip_multline_comment()
{

}
