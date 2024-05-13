#include "include/lexer.h"

#include <iostream>
#include <fstream>


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

  return 0;
}
