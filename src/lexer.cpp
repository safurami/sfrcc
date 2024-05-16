/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

#include <iostream>

lexer::lexer(char *f)
{
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
  if(this->buffer1 == nullptr)
    return false;
  return true;
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

void lexer::fail()
{
  std::cout << "Lexical error on line " << this->current_line << std::endl;
  //printf("Lexical error on line %d\n", this->current_line);
  if(this->file.is_open())
  {
    this->file.close();
  }
  delete this->current_token;
  delete[] this->buffer1;
  delete[] this->buffer2;
  exit(1);
}

bool lexer::match(char sym)
{
  if(*this->forward == sym) { forward++; return true; }
  return false;
  //return *this->forward++ == sym;
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

int lexer::collect_int()
{
  // TODO
  return 0;
}

int lexer::collect_id()
{
  // TODO
  return 0;
}

int lexer::collect_literal()
{
  // TODO
  return 0;
}

token* lexer::get_next_token()
{
  char symbol = *forward++;
  switch(symbol)
  {
    case '\0':
      if(this->forward == this->buffer1 + 4096)
      {
        this->file.read(this->buffer2, 4096);
        this->forward = this->buffer2;
      }
      else if(this->forward == this->buffer2 + 4096)
      {
        this->file.read(this->buffer1, 4096);
        this->forward = this->buffer1;
      }
      else
      {
        this->current_token->set_type(token_type::DOLLAR);
      }
      break;
    case '\n':
      this->current_line++; break;
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
      this->current_token->set_type( this->match('=') ? token_type::IS_EQUAL : token_type::ASSIGN);
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
      this->current_token->set_type(token_type::DIVIDE);
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
      else if(this->check_word("nt") &&
              !my::isalnum(*forward)) // possible bug
      {
        this->current_token->set_type(token_type::INT);
      }
      else
      {
        // this->forward = this->lexeme_begin; // to reset read symbols MOVE TO collect_id
        this->collect_id();
        // this->current_token->set_type(token_type::IDENTIFIER); MOVE TO collect_id
      }
      break;
    case '\'':
      this->forward++;
      if(!match('\''))
      {
        this->fail();
      }
      this->current_token->set_type(token_type::CONS);
      break;
    case '"':
      this->collect_literal();
      this->current_token->set_type(token_type::LITERAL);
      break;
    case 'c':
      if(this->check_word("har") &&
         !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CHAR);
      }
      else if((this->forward = this->lexeme_begin),
              this->check_word("onst") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CONST);
      }
      else if((this->forward = this->lexeme_begin),
              this->check_word("ace") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::CASE);
      }
      else
      {
        this->collect_id();
        //this->current_token->set_type(token_type::IDENTIFIER); MOVE TO collect_id
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
      else if((this->forward = this->lexeme_begin),
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
      else if((this->forward = this->lexeme_begin),
              this->check_word("hort") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::SHORT);
      }
      else if((this->forward = this->lexeme_begin),
              this->check_word("witch") &&
              !my::isalnum(*forward))
      {
        this->current_token->set_type(token_type::SWITCH);
      }
      else if((this->forward = this->lexeme_begin),
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
        this->collect_int();
      }
      break;
  }

  this->lexeme_begin = this->forward; // possible bug
  return this->current_token;
}
