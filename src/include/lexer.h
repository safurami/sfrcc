#ifndef H_LEXER
#define H_LEXER

#include "./token.h"
#include <fstream>

class lexer_handler
{
  int current_line;
  char *storage;
  char *input;
  token* current_token;
  std::ifstream file;
public:
  lexer_handler(lexer_handler&) = delete;
  lexer_handler(char *); // * - file
  ~lexer_handler();
  token* get_next_token();
};


#endif
