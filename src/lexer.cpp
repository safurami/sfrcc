/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

lexer::lexer(const char *f, symbol_table* table)
{
  if(table == nullptr)
  {
    this->m_buffer1 = nullptr;
    return;
  }
  this->m_table = table;
  this->m_file.open(f);

  if(!m_file.is_open())
  {
    this->m_buffer1 = nullptr; // Error, nothing is allocating
    this->m_buffer_2 = nullptr;
    this->m_current_token = nullptr;
    return;
  }
  this->m_current_token = new token();
  this->m_buffer1 = new char[4097](); // 4096 + 1 for null byte
  this->m_buffer_2 = new char[4097]();
  this->m_file.read(this->m_buffer1, 4096);
  this->m_lexeme_begin = m_buffer1;
  this->m_forward = m_buffer1;
  this->m_current_line = 1;
}

bool lexer::is_success()
{
  return this->m_buffer1 != nullptr;
}

lexer::~lexer()
{
  if(this->m_file.is_open())
  {
    this->m_file.close();
  }
  delete this->m_current_token;
  delete[] this->m_buffer1;
  delete[] this->m_buffer_2;
}

int lexer::get_current_line()
{
  return this->m_current_line;
}

void lexer::fail() // TODO: possible memory leak i guess, and prefered to refactor this function.
{
  this->m_forward--; // decrement, because next_character() incrementing forward pointer, so forward pointer to next character

  printf("\n---Unexpected character '%c' on line %d---\n", *this->m_forward, this->m_current_line);

  char *string_start;


  if(this->m_current_line == 1)
  {
    string_start = this->m_buffer1;
  }
  else
  {
    for(;*(this->m_lexeme_begin-1) != '\n'; this->m_lexeme_begin--) {}
    string_start = this->m_lexeme_begin;
  }
  for(this->m_lexeme_begin = string_start; *this->m_lexeme_begin != '\n'; this->m_lexeme_begin++) { printf("%c", *this->m_lexeme_begin); }
  printf("\n");
  for(this->m_lexeme_begin = string_start; this->m_lexeme_begin != this->m_forward;this->m_lexeme_begin++)
  {
    printf("~");
  }
  printf("^\n");

  if(this->m_file.is_open())
  {
    this->m_file.close();
  }
  delete this->m_current_token;
  delete[] this->m_buffer1;
  delete[] this->m_buffer_2;
  delete this->m_table;
  exit(1);
}

bool lexer::match(char sym)
{
  return this->next_character() == sym;
}

bool lexer::check_word(const char *word)
{
  for(int i = 0; word[i] != '\0'; i++)
  {
    if(!this->match(word[i]))
    {
       return false;
    }
  }
  return true;
}

void lexer::collect_number()
{
  this->m_forward = this->m_lexeme_begin;
  for(;my::isdigit(*m_forward); this->next_character()) {}
  this->m_current_token->set_type(token_type::NUMBER);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin, this->m_forward - 1, node_type::NUMBER)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_id()
{
  this->m_forward = this->m_lexeme_begin;
  for(; my::isalnum(*this->m_forward); this->next_character()) {}
  this->m_current_token->set_type(token_type::IDENTIFIER);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin, this->m_forward - 1, node_type::IDENTIFIER)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_literal()
{
  this->reset_lexeme();
  while(this->next_character() != '"') {}
  this->m_current_token->set_type(token_type::LITERAL);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin + 1, this->m_forward - 2, node_type::LITERAL)); // TODO: fix loop, loop reads two mode character than needed.
}

/*
 * Function for getting current character and increasing
 * m_forward pointer, if m_forward pointer now points to null byte -
 * check, if it is an end of either buffer, then read m_file to another buffer
 * and switch m_forward pointer there, otherwise return EOF. If it is not null byte -
 * return character that was read.
 */
char lexer::next_character()
{
  char *tmp;
  for(; (tmp = this->m_forward), *tmp == '\0' || *tmp == '\n';)
  {
    if(*tmp == '\0')
    {
      if(tmp == this->m_buffer1 + 4096)
      {
        this->m_file.read(this->m_buffer_2, 4096);
        this->m_forward = this->m_buffer_2;
      }
      else if(tmp == this->m_buffer_2 + 4096)
      {
        this->m_file.read(this->m_buffer1, 4096);
        this->m_forward = this->m_buffer1;
      }
      else
      {
        return EOF;
      }
    }
    if(*tmp == '\n')
    {
      this->m_current_line++;
      this->m_forward++;
      this->m_lexeme_begin++; // without this increment, lexeme_begin will point to '\n', and check_word() will alway fail.
    }
  }
  this->m_forward++;
  return *tmp;
}

void lexer::skip_line_comment()
{
  // TODO: implemented via raw pointer. That will spoil all when buffer1 will end. Implement via next_character().
  for(;*this->m_forward++ != '\n';) {}
  this->m_current_line++; // New line occurred
  this->m_lexeme_begin = this->m_forward;
}

void lexer::skip_multline_comment()
{
  while(1)
  {
    if(this->match('*') && this->match('/'))
    {
      break;
    }
  }
  this->m_lexeme_begin = this->m_forward;
}

/*
 * Function to reset lexeme.
 * This function assign this->m_forward to this->m_lexeme_begin + 1.
 * +1 because first symbol is already read by switch statement in
 * get_next_token().
 * Returns true just for comfortable using in if statements.
 */
bool lexer::reset_lexeme()
{
  this->m_forward = this->m_lexeme_begin + 1;
  return true;
}

token* lexer::get_next_token()
{
  char symbol;
again: // label to avoid returning token after whitespace or comment
  switch(symbol = this->next_character())
  {
    case EOF:
      this->m_current_token->set_type(token_type::DOLLAR);
      break;
    case ' ': this->m_lexeme_begin = this->m_forward; goto again;
    case '\t': this->m_lexeme_begin = this->m_forward; goto again;
    case '(':
      this->m_current_token->set_type(token_type::OPEN_PAREN);
      break;
    case ')':
      this->m_current_token->set_type(token_type::CLOSE_PAREN);
      break;
    case ';':
      this->m_current_token->set_type(token_type::SEMICOLON);
      break;
    case '{':
      this->m_current_token->set_type(token_type::OPEN_CURLYB);
      break;
    case '}':
      this->m_current_token->set_type(token_type::CLOSE_CURLYB);
      break;
    case '=':
      if(this->match('='))
      {
        this->m_current_token->set_type(token_type::IS_EQUAL);
      }
      else
      {
        this->reset_lexeme();
        this->m_current_token->set_type(token_type::ASSIGN);
      }
      break;
    case ',':
      this->m_current_token->set_type(token_type::COMA);
      break;
    case '[':
      this->m_current_token->set_type(token_type::SQ_OPEN_B);
      break;
    case ']':
      this->m_current_token->set_type(token_type::SQ_CLOSE_B);
      break;
    case '+':
      this->m_current_token->set_type(token_type::PLUS);
      break;
    case '-':
      this->m_current_token->set_type(token_type::MINUS);
      break;
    case '*':
      this->m_current_token->set_type(token_type::STAR);
      break;
    case '/':
      if(this->match('/'))
      {
        this->skip_line_comment();
        goto again;
      }
      else if(this->reset_lexeme() && this->match('*'))
      {
        this->skip_multline_comment();
        goto again;
      }
      else
      {
        this->reset_lexeme();
        this->m_current_token->set_type(token_type::DIVIDE);
      }
      break;
    case '<':
      this->m_current_token->set_type( this->match('=') ? token_type::LEQ : token_type::LESS);
      break;
    case '>':
      this->m_current_token->set_type( this->match('=') ? token_type::GEQ : token_type::GREATER);
      break;
    case '.':
      this->m_current_token->set_type(token_type::DOT);
      break;
    case '!':
      if(this->match('='))
      {
        this->m_current_token->set_type(token_type::NEQ);
      } else {
        this->fail();
      }
      break;
    case 'i':
      if(this->match('f') && !my::isalnum(*m_forward)) // possible bug
      {
        this->m_current_token->set_type(token_type::IF);
      }
      else if(this->reset_lexeme() &&
              this->check_word("nt") &&
              !my::isalnum(*m_forward)) // possible bug
      {
        this->m_current_token->set_type(token_type::INT);
      }
      else
      {
        this->collect_id();
      }
      break;
    case '\'':
      this->m_current_token->set_attribute(this->next_character());
      if(!match('\''))
      {
        this->fail();
      }
      this->m_current_token->set_type(token_type::CONS);
      break;
    case '"':
      this->collect_literal();
      break;
    case 'c':
      if(this->check_word("har") &&
         !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::CHAR);
      }
      else if(this->reset_lexeme() &&
              this->check_word("onst") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::CONST);
      }
      else if(this->reset_lexeme() &&
              this->check_word("ase") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::CASE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'v':
      if(this->check_word("oid") &&
         !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::VOID);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'u':
      if(this->check_word("nsigned") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::UNSIGNED);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'f':
      if(this->check_word("loat") &&
         !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::FLOAT);
      }
      else if(this->reset_lexeme() &&
              this->check_word("or") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::FOR);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'd':
      if(this->check_word("ouble") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::DOUBLE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 's':
      if(this->check_word("igned") &&
         !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::SIGNED);
      }
      else if(this->reset_lexeme() &&
              this->check_word("hort") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::SHORT);
      }
      else if(this->reset_lexeme() &&
              this->check_word("witch") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::SWITCH);
      }
      else if(this->reset_lexeme() &&
              this->check_word("truct") &&
              !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::STRUCT);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'e':
      if(this->check_word("lse") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::ELSE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'r':
      if(this->check_word("eturn") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::RETURN);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'w':
      if(this->check_word("hile") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::WHILE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'b':
      if(this->check_word("reak") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::BREAK);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'l':
      if(this->check_word("ong") && !my::isalnum(*m_forward))
      {
        this->m_current_token->set_type(token_type::LONG);
      }
      else
      {
        this->collect_id();
      }
      break;
    case '&':
      this->m_current_token->set_type(token_type::AND);
      break;
    case '|':
      this->m_current_token->set_type(token_type::OR);
      break;
    default:
      if(my::isalpha(symbol))
      {
        this->collect_id();
      }
      else if(my::isdigit(symbol))
      {
        this->collect_number();
      }
      else // Something else, for example $ or %
      {
        this->fail();
      }
      break;
  }

  this->m_lexeme_begin = this->m_forward;
  return this->m_current_token;
}
