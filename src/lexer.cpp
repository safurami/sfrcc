#include <fstream>
#include <iostream>
#include <string>

#include "./include/token.h"
#include "./include/lexer.h"

void start_lexer(token *token, std::ifstream &file)
{
  std::cout << "it is working" << std::endl;
  /*std::string buffer;   // buffer for string from file
  const char* ptr;    // pointer to convert std::string to char *

  while(getline(file, buffer))  // loop until there are no lines in the file
  {
    ptr = buffer.c_str(); // convert std::string to char *
    while(*ptr != '\0') // loop until end of line in file
    {
      get_token(ptr, token);
    }
  }*/
}
