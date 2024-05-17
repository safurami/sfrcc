/*
 * Functions for working with token
 */

#include "include/token.h"
#include "include/my.h"

void token::set_type(token_type type)
{
  this->type = type;
}

int token::get_type() // REMOVEME return data type to token_type
{
  return static_cast<int>(this->type);
  //return this->type;
}

void token::set_attribute(int attribute)
{
  this->attribute = attribute;
}

int token::get_attribute()
{
  return this->attribute;
}
