/*
 * Functions for working with token
 */

#include "include/token.h"

void token::set_type(token_type type)
{
  this->type = type;
}

token_type token::get_type()
{
  return this->type;
}

void token::set_attribute(int attribute)
{
  this->attribute = attribute;
}

int token::get_attribute()
{
  return this->attribute;
}
