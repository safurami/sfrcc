/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

lexer::lexer(char *f, symbol_table* table)
{
  if(table == nullptr)
  {
    this->buffer1 = nullptr;
    return;
  }
  this->table = table;
  this->file.open(f);

  if(!file.is_open())
  {
    this->buffer1 = nullptr; // Error, nothing is allocating
    this->buffer2 = nullptr;
    this->current_token = nullptr;
    return;
  }
  this->current_token = new token();
  this->buffer1 = new char[4097](); // 4096 + 1 for null byte
  this->buffer2 = new char[4097]();
  this->file.read(this->buffer1, 4096);
  this->lexeme_begin = buffer1;
  this->forward = buffer1;
  this->current_line = 1;
}

bool lexer::is_success()
{
  return this->buffer1 != nullptr;
}

lexer::~lexer()
{
  if(this->file.is_open())
  {
    this->file.close();
  }
  delete this->current_token;
  delete[] this->buffer1;
  delete[] this->buffer2;
}

int lexer::get_current_line()
{
  return this->current_line;
}

void lexer::fail() // TODO: possible memory leak i guess, and prefered to refactor this function.
{
  this->forward--;
  char *string_start;

  printf("\n---Unexpected token on line: %d---\n\n", this->current_line);

  if(this->current_line == 1)
  {
    string_start = this->buffer1;
  }
  else
  {
    for(;*(this->lexeme_begin-1) != '\n'; this->lexeme_begin--) {}
    string_start = this->lexeme_begin;
  }
  for(this->lexeme_begin = string_start; *this->lexeme_begin != '\n'; this->lexeme_begin++) { printf("%c", *this->lexeme_begin); }
  printf("\n");
  for(this->lexeme_begin = string_start; this->lexeme_begin != this->forward;this->lexeme_begin++)
  {
    printf("~");
  }
  printf("^\n");

  if(this->file.is_open())
  {
    this->file.close();
  }
  delete this->current_token;
  delete[] this->buffer1;
  delete[] this->buffer2;
  delete this->table;
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
  this->forward = this->lexeme_begin;
  for(;my::isdigit(*forward); this->next_character()) {}
  this->current_token->set_type(token_type::NUMBER);
  this->current_token->set_attribute(this->table->install_id(this->lexeme_begin, this->forward - 1)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_id()
{
  this->forward = this->lexeme_begin;
  for(; my::isalnum(*this->forward); this->next_character()) {}
  this->current_token->set_type(token_type::IDENTIFIER);
  this->current_token->set_attribute(this->table->install_id(this->lexeme_begin, this->forward - 1)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_literal()
{
  this->reset_lexeme();
  while(this->next_character() != '"') {}
  this->current_token->set_type(token_type::LITERAL);
  this->current_token->set_attribute(this->table->install_id(this->lexeme_begin + 1, this->forward - 2)); // TODO: fix loop, loop reads two mode chatacter than needed.
}

/*
 * Function for getting current character and increasing
 * forward pointer, if forward pointer now points to null byte -
 * check, if it is an end of either buffer, then read file to another buffer
 * and move forward pointer there, otherwise return EOF. If it is not null byte -
 * return character that was read.
 */
char lexer::next_character()
{
  char *tmp;
  for(; (tmp = forward), *tmp == '\0' || *tmp == '\n';)
  {
    if(*tmp == '\0')
    {
      if(tmp == this->buffer1 + 4096)
      {
        this->file.read(this->buffer2, 4096);
        this->forward = this->buffer2;
      }
      else if(tmp == this->buffer2 + 4096)
      {
        this->file.read(this->buffer1, 4096);
        this->forward = this->buffer1;
      }
      else
      {
        return EOF;
      }
    }
    if(*tmp == '\n')
    {
      this->current_line++;
      this->forward++;
      this->lexeme_begin++; // without this increment, lexeme_begin will point to '\n', and check_word() will alway fail.
    }
  }
  this->forward++;
  return *tmp;
}

void lexer::skip_line_comment()
{
  // TODO: implemented via raw pointer. That will spoil all when buffer1 will end. Implement via next_character().
  for(;*this->forward++ != '\n';) {}
  this->current_line++; // New line occurred
  this->lexeme_begin = this->forward;
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
  this->lexeme_begin = this->forward;
  // TODO: implement skipping multiple lines comment
}

/*
 * Function to reset lexeme.
 * This function assign this->forward to this->lexeme_begin + 1.
 * +1 because first symbol is already read by switch statement in
 * get_next_token().
 * Returns true just for comfortable using in if statements.
 */
bool lexer::reset_lexeme()
{
  this->forward = this->lexeme_begin + 1;
  return true;
}

token* lexer::get_next_token() // TODO, imlement checking to end of buffer in case statement
{
  char symbol;
again: // label to avoid returning token after whitespace or comment
  switch(symbol = this->next_character())
  {
    case EOF:
      this->current_token->set_type(token_type::DOLLAR);
      break;
    case ' ': this->lexeme_begin = this->forward; goto again;
    case '\t': this->lexeme_begin = this->forward; goto again;
    case '(':
      this->current_token->set_type(token_type::OPEN_PAREN);
      break;
    case ')':
      this->current_token->set_type(token_type::CLOSE_PAREN);
      break;
    case ';':
      this->current_token->set_type(token_type::SEMICOLON);
      break;
    case '{':
      this->current_token->set_type(token_type::OPEN_CURLYB);
      break;
    case '}':
      this->current_token->set_type(token_type::CLOSE_CURLYB);
      break;
    case '=':
      if(this->match('='))
      {
        this->current_token->set_type(token_type::IS_EQUAL);
      }
      else
      {
        this->reset_lexeme();
        this->current_token->set_type(token_type::ASSIGN);
      }
      break;
    case ',':
      this->current_token->set_type(token_type::COMA);
      break;
    case '[':
      this->current_token->set_type(token_type::SQ_OPEN_B);
      break;
    case ']':
      this->current_token->set_type(token_type::SQ_CLOSE_B);
      break;
    case '+':
      this->current_token->set_type(token_type::PLUS);
      break;
    case '-':
      this->current_token->set_type(token_type::MINUS);
      break;
    case '*':
      this->current_token->set_type(token_type::STAR);
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
        this->current_token->set_type(token_type::DIVIDE);
      }
      break;
    case '<':
      this->current_token->set_type( this->match('=') ? token_type::LEQ : token_type::LESS);
      break;
    case '>':
      this->current_token->set_type( this->match('=') ? token_type::GEQ : token_type::GREATER);
      break;
    case '.':
      this->current_token->set_type(token_type::DOT);
      break;
    case '!':
      if(this->match('='))
      {
        this->current_token->set_type(token_type::NEQ);
      } else {
        this->fail();
      }
      break;
    case 'i':
      if(this->match('f') && !my::isalnum(*forward)) // possible bug
      {
        this->current_token->set_type(token_type::IF);
      }
      else if(this->reset_lexeme() &&
              this->check_word("nt") &&
              !my::isalnum(*forward)) // possible bug
      {
        this->current_token->set_type(token_type::INT);
      }
      else
      {
        this->collect_id();
      }
      break;
    case '\'':
      this->current_token->set_attribute(this->next_character());
      if(!match('\''))
      {
        this->fail();
      }
      this->current_token->set_type(token_type::CONS);
      break;
    case '"':
      this->collect_literal();
      break;
    case 'c':
      if(this->check_word("har") &&
         !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CHAR);
      }
      else if(this->reset_lexeme() &&
              this->check_word("onst") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CONST);
      }
      else if(this->reset_lexeme() &&
              this->check_word("ase") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CASE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'v':
      if(this->check_word("oid") &&
         !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::VOID);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'u':
      if(this->check_word("nsigned") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::UNSIGNED);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'f':
      if(this->check_word("loat") &&
         !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::FLOAT);
      }
      else if(this->reset_lexeme() &&
              this->check_word("or") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::FOR);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'd':
      if(this->check_word("ouble") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::DOUBLE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 's':
      if(this->check_word("igned") &&
         !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::SIGNED);
      }
      else if(this->reset_lexeme() &&
              this->check_word("hort") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::SHORT);
      }
      else if(this->reset_lexeme() &&
              this->check_word("witch") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::SWITCH);
      }
      else if(this->reset_lexeme() &&
              this->check_word("truct") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::STRUCT);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'e':
      if(this->check_word("lse") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::ELSE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'r':
      if(this->check_word("eturn") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::RETURN);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'w':
      if(this->check_word("hile") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::WHILE);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'b':
      if(this->check_word("reak") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::BREAK);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'l':
      if(this->check_word("ong") && !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::LONG);
      }
      else
      {
        this->collect_id();
      }
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
      break;
  }

  this->lexeme_begin = this->forward;
  return this->current_token;
}
