#include "include/parser.h"
#include "include/ast.h"
#include "include/my.h"

parser::parser(lexer *lex, symbol_table *table)
{
  this->m_lexer = lex;
  this->m_table = table;
  this->m_was_error = false;
}

void parser::set_input(token *input)
{
  this->m_input = input;
}

token& parser::advance()
{
  if(this->m_input->get_type() != token_type::DOLLAR)
  {
    this->m_input++;
  }
  return this->previous();
}

bool parser::match(token_type type)
{
  if(this->check(type))
  {
    this->advance();
    return true;
  }
  return false;
}

bool parser::check(token_type type)
{
  return this->peek().get_type() == type;
}

token& parser::previous()
{
  return *(this->m_input - 1);
}

token& parser::peek()
{
  return *this->m_input;
}

bool parser::was_error()
{
  return this->m_was_error;
}


