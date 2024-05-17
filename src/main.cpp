#include "include/lexer.h"
#include "include/token.h" // REMOVEME
#include "include/my.h" // REMOVEME

#include <iostream>


int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cout << "Usege: " << argv[0] << " <file(s)>\n";
    return 1;
  }

  lexer lex(argv[1]);
  if(!lex.is_success())
  {
    return 1;
  }

  for(int i = 0; (i = lex.get_next_token()->get_type()) != (int)token_type::DOLLAR;)
  {
    printf("Line : %d\tType: %d\n", lex.get_current_line(), i);
  }

  return 0;
}
