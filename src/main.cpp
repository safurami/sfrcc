#include "include/lexer.h"
#include "include/parser.h"
#include "include/my.h"

static const char* token_type_to_string(token_type t) // REMOVEME, just for debug
{
  switch((int)t)
  {
  case (int)token_type::OPEN_PAREN: return "Open paren";
  case (int)token_type::CLOSE_PAREN: return "Close paren";
  case (int)token_type::SEMICOLON: return "Semicolon";
  case (int)token_type::OPEN_CURLYB: return "Open curly";
  case (int)token_type::CLOSE_CURLYB: return "Close curly";
  case (int)token_type::ASSIGN: return "Assign";
  case (int)token_type::COMA: return "Coma";
  case (int)token_type::SQ_OPEN_B: return "Square open";
  case (int)token_type::SQ_CLOSE_B: return "Square close";
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
  }
  my_assert(false);
  return "FAIL";
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Usage: %s <file(s)>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // For testing lexer!
  /*
  symbol_table* t = new symbol_table();
  lexer lex(argv[1], t);

  if(!lex.is_success())
  {
    return EXIT_FAILURE;
  }

  token* tok;

  for(; (tok = lex.get_next_token()), static_cast<int>(tok->get_type()) != static_cast<int>(token_type::DOLLAR); )
  {
    if(tok->get_type() != token_type::IDENTIFIER && tok->get_type() != token_type::LITERAL && tok->get_type() != token_type::NUMBER && tok->get_type() != token_type::CONS)
    {
      printf("<Line = %d, Type = %s>\n", lex.get_current_line(), token_type_to_string(tok->get_type()));
    }
    else
    {
      printf("<Line = %d, Type = %s, Attribute = %d>\n", lex.get_current_line(), token_type_to_string(tok->get_type()), tok->get_attribute());
    }
  }

  printf("\n");
  t->dump_table();

  delete t;
  */

  symbol_table t;

  lexer l(argv[1], &t);

  parse(&t, &l);

  return EXIT_SUCCESS;
}
