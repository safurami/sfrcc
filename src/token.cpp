/*
 * Functions for working with token
 */

#include "include/token.h"
#include "include/my.h"

token::token()
{
  this->lexeme = nullptr;
}

token::~token()
{
  delete[] this->lexeme;
}

void token::set_type(token_type_t type)
{
  this->token_type = type;
}

void token::set_lexeme(const char *begin, const char* end)
{
  if(this->lexeme != nullptr)
  {
    delete[] this->lexeme;
  }
  this->lexeme = new char[end - begin + 1];
  my::strncpy(this->lexeme, begin, end - begin);
}

char* token::get_lexeme()
{
  return this->lexeme;
}

token_type_t token::get_type()
{
  return this->token_type;
}

