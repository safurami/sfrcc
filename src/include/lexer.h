#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <fstream>
#include <vector>

class Lexer
{
public:
  int line;
  char putback;
  std::ifstream *file;
  bool was_error;

  Lexer(std::ifstream *file);
  char skip(void);
  char next(void);
  void putback_char(const char c);
  int get_tok(token *tok);
  int scanint(char val);
};


std::vector<token> *scan(Lexer *lexer);

#endif /* SCANNER_H */
