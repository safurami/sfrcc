/*
 * Functions for working with token
 */

#include "include/token.h"
#include <iostream>

token::token(const token& other) // copy constructor
{
  this->token_type = other.token_type;
  this->attribute = other.attribute;
}

void token::print_info()
{
  std::cout << "<Class: "; 
  switch(this->token_type)
  {
    case(OPEN_PAREN):
      std::cout << "OPEN PAREN";
      break;
    case(CLOSE_PAREN):
      std::cout << "CLOSE PAREN";
      break;
    case(SEMICOLON):
      std::cout << "SEMICOLON";
      break;
    case(OPEN_CURLYB):
      std::cout << "OPEN CURLY BRACE";
      break;
    case(CLOSE_CURLYB):
      std::cout << "CLOSE CURLY BRACE";
      break;
    case(EQUAL):
      std::cout << "EQUAL";
      break;
    case(COMA):
      std::cout << "COMA";
      break;
    case(SQ_OPEN_B):
      std::cout << "SQUARE OPEN BRACE";
      break;
    case(SQ_CLOSE_B):
      std::cout << "SQUARE CLOSE BRACE";
      break;
    case(QUOTE):
      std::cout << "QUOTE";
      break;
    case(PLUS):
      std::cout << "PLUS";
      break;
    case(MINUS):
      std::cout << "MINUS";
      break;
    case(STAR):
      std::cout << "START";
      break;
    case(DIVIDE):
      std::cout << "DIVIDE";
      break;
    case(DOUBLE_EQUAL):
      std::cout << "DOUBLE EQUAL";
      break;
    case(IDENTIFIER):
      std::cout << "IDENTIFIER";
      break;
    case(NUMBER):
      std::cout << "NUMBER";
      break;
    case(INT):
      std::cout << "INT";
      break;
    case(CHAR):
      std::cout << "CHAR";
      break;
    case(UNSIGNED):
      std::cout << "UNSIGNED";
      break;
    case(IF):
      std::cout << "IF";
      break;
    case(ELSE):
      std::cout << "ELSE";
      break;
    case(RETURN):
      std::cout << "RETURN";
      break;
    case(CONST):
      std::cout << "CONST";
      break;
    case(FOR):
      std::cout << "FOR";
      break;
    case(WHILE):
      std::cout << "WHILE";
      break;
    case(SWITCH):
      std::cout << "SWITCH";
      break;
    case(LONG):
      std::cout << "LONG";
      break;
    case(CASE):
      std::cout << "CASE";
      break;
    case(STRUCT):
      std::cout << "STRUCT";
      break;
  }
  std::cout << ",\'" << attribute << "\'>" << '\n';
}

void token::set_type(token_type_t type)
{
  this->token_type = type;
}

void token::set_attribute(int attr)
{
  this->attribute = attr;
}

token *init_token()
{
  token* init = new token;
  return init;
}

void free_token(token *tok)
{
  delete tok;
}
