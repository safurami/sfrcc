#include "include/lexer.h"
#include "include/token.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Usage: %s <file(s)>\n", argv[0]);
    return 1;
  }

  // --- DEBUG ---
  symbol_table* t = new symbol_table();
  lexer lex(argv[1], t);

  if(!lex.is_success())
  {
    return 1;
  }

  token* tok;

  for(int i = 0; tok = lex.get_next_token(), ((i = static_cast<int>(tok->get_type())) != static_cast<int>(token_type::DOLLAR));)
  {
    if(static_cast<int>(tok->get_type()) == static_cast<int>(token_type::CONS))
      printf("Line: %02d\tType: %02d\tAttribute: %d\n", lex.get_current_line(), i, tok->get_attribute());
    else if(static_cast<int>(tok->get_type()) == static_cast<int>(token_type::LITERAL) ||
             static_cast<int>(tok->get_type()) == static_cast<int>(token_type::NUMBER) ||
             static_cast<int>(tok->get_type()) == static_cast<int>(token_type::IDENTIFIER))
      printf("Line: %02d\tType: %02d\tAttribute: %d\n", lex.get_current_line(), i, tok->get_attribute());
    else
      printf("Line: %02d\tType: %02d\n", lex.get_current_line(), i);
  }

  printf("\n");
  t->dump_table();

  delete t;
  // -- DEBUG ---

  return 0;
}
