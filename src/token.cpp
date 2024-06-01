/*
 * Functions for working with token
 */

#include "include/token.h"

#include <iostream>

token::token()
{
  this->m_type = token_type::VOID;
  this->m_attribute = 0;
}

void token::set_type(token_type type)
{
  this->m_type = type;
}

token_type token::get_type()
{
  return this->m_type;
}

void token::set_attribute(int attribute)
{
  this->m_attribute = attribute;
}

int token::get_attribute()
{
  return this->m_attribute;
}

void token::operator =(const token& other)
{
  std::cout << "REMOVEME: ASSIGNMENT OPERATOR WAS CALLED\n";
  this->m_attribute = other.m_attribute;
  this->m_type = other.m_type;
}
