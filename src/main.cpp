#include "include/lexer.h"
#include "include/token.h" // REMOVEME

#include <iostream>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cout << "Usege: " << argv[0] << " <file(s)>\n";
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

  for(int i = 0; tok = lex.get_next_token(), ((i = tok->get_type()) != static_cast<int>(token_type::DOLLAR));)
  {
    if(tok->get_type() == static_cast<int>(token_type::CONS))
      printf("Line: %02d\tType: %02d\tAttribute: %c\n", lex.get_current_line(), i, tok->get_attribute());
    else
      printf("Line: %02d\tType: %02d\tAttribute: %d\n", lex.get_current_line(), i, tok->get_attribute());
  }

  std::cout << std::endl;
  t->dump_table();

  delete t;
  // -- DEBUG ---

  return 0;
}
