#ifndef H_LEXER
#define H_LEXER

#include "token.h"
#include "symbol_table.h"
#include <fstream>

class lexer
{
  int current_line;
  char* buffer1;
  char* buffer2;
  char* lexeme_begin; // pointer to point to start of the current lexeme
  char* forward; // pointer to read lexemes
  token* current_token; // REMOVEME maybe remove it, and use local token in get_next_token()
  std::ifstream file;
public:
  lexer(lexer&) = delete;
  lexer(char *); // * - file
  ~lexer();
  bool is_success();
  int get_current_line();
  void fail();
  token* get_next_token();
  bool check_word(const char *);
  bool match(char);
  int collect_number();
  int collect_id();
  int collect_literal();
  bool reset_lexeme();
};


#endif
