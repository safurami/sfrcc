#include <iostream>
#include <fstream>

#include "./include/token.h"
#include "./include/lexer.h"

int main(int argc, char *argv[])
{
  std::ifstream file;
  if(argc < 2)
  {
    std::cout << "Usege: " << argv[0] << " <file(s)>\n";
    return 1;
  }

  file.open(argv[1]);   // opening file

  if(!file.is_open())
  {
    std::cout << "File \"" << argv[1] << "\" Not Found\n";
    return 1;
  }

  token* token = init_token();

  start_lexer(token, file); // start lexical analisys

  free_token(token);

  return 0;
}
