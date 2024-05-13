/*
 * Functions for working with token
 */

#include "include/token.h"
#include "include/my.h"

void token::set_type(token_type_t type)
{
  this->token_type = type;
}

token_type_t token::get_type()
{
  return this->token_type;
}
