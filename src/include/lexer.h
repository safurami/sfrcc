#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <vector>

#include "token.h"
#include "my.h"

// Special macros to use these functions in case of identifiers in C,
// because _ in C may be the part of the identifier.
#define isalnum_id(c) ((c) == '_' ? true : my::isalnum(c))
#define isalpha_id(c) ((c) == '_' ? true : my::isalpha(c))

class Lexer
{
public:
  int line;
  char putback;
  std::ifstream *file;
  bool was_error;
  my::vector<char> buffer;

  Lexer(std::ifstream *file);
  char skip(void);
  char next(void);
  void putback_char(const char c);
  int get_tok(token *tok);
  int scan_int(char val);
  void scan_ident(char val);
  token_type analize_buffer();
};


std::vector<token> *scan(Lexer *lexer);

#endif /* SCANNER_H */
