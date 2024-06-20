/*
 * Functions for working with token
 */

#include "include/token.h"

token::token()
{
  this->m_type = token_type::VOID;
  this->m_attribute = 0;
  this->m_line = 0;
}

token::token(token& other)
{
  this->m_attribute = other.m_attribute;
  this->m_type = other.m_type;
  this->m_line = other.m_line;
}

void token::set_parameters(token_type type, int line)
{
  this->m_type = type;
  this->m_line = line;
  if(type != token_type::LITERAL && type != token_type::IDENTIFIER && type != token_type::NUMBER && type != token_type::CONS) // just for comfort.
  {
    this->m_attribute = 0;
  }
}

int token::get_line()
{
  return this->m_line;
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
  this->m_attribute = other.m_attribute;
  this->m_type = other.m_type;
  this->m_line = other.m_line;
}

const char* token_type_to_string(token_type t) // REMOVEME, just for debug
{
  switch((int)t)
  {
  case (int)token_type::OPEN_PAREN: return "Open paren";
  case (int)token_type::CLOSE_PAREN: return "Close paren";
  case (int)token_type::SEMICOLON: return "Semicolon";
  case (int)token_type::OPEN_CURLYB: return "Open curly";
  case (int)token_type::CLOSE_CURLYB: return "Close curly";
  case (int)token_type::ASSIGN: return "Assign";
  case (int)token_type::COMMA: return "Comma";
  case (int)token_type::SQ_OPENB: return "Square open";
  case (int)token_type::SQ_CLOSEB: return "Square close";
  case (int)token_type::PLUS: return "Plus";
  case (int)token_type::MINUS: return "Minus";
  case (int)token_type::STAR: return "Star";
  case (int)token_type::DIVIDE: return "Divide";
  case (int)token_type::LESS: return "Less";
  case (int)token_type::GREATER: return "Greater";
  case (int)token_type::DOLLAR: return "End of file";
  case (int)token_type::DOT: return "Dot";
  case (int)token_type::AND: return "And";
  case (int)token_type::OR: return "Or";
  case (int)token_type::IS_EQUAL: return "Is equal";
  case (int)token_type::LEQ: return "Less or equal";
  case (int)token_type::GEQ: return "Greater or equal";
  case (int)token_type::NEQ: return "Not equal";
  case (int)token_type::IDENTIFIER: return "Identifier";
  case (int)token_type::NUMBER: return "Number";
  case (int)token_type::CONS: return "Constant value";
  case (int)token_type::LITERAL: return "Literal";
  case (int)token_type::INT: return "Int keyword";
  case (int)token_type::CHAR: return "Char keyword";
  case (int)token_type::VOID: return "Void keyword";
  case (int)token_type::UNSIGNED: return "Unsigned keyword";
  case (int)token_type::FLOAT: return "Float keyword";
  case (int)token_type::DOUBLE: return "Double keyword";
  case (int)token_type::SIGNED: return "Signed keyword";
  case (int)token_type::IF: return "If keyword";
  case (int)token_type::SHORT: return "Short keyword";
  case (int)token_type::ELSE: return "Else keyword";
  case (int)token_type::RETURN: return "Return keyword";
  case (int)token_type::CONST: return "Const keyword";
  case (int)token_type::FOR: return "For keyword";
  case (int)token_type::WHILE: return "While keyword";
  case (int)token_type::BREAK: return "Break keyword";
  case (int)token_type::SWITCH: return "Switch keyword";
  case (int)token_type::LONG: return "Long keyword";
  case (int)token_type::CASE: return "Case keyword";
  case (int)token_type::STRUCT: return "Struct keyword";
  case (int)token_type::BANG: return "Bang";
  case (int)token_type::COLON: return "Colon";
  }
  return "FAIL";
}
