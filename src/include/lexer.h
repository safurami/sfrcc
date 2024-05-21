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
  token* current_token;
  std::ifstream file;
public:
  symbol_table* table; // TODO: bring me back to private section.
  lexer(lexer&) = delete;
  lexer(char *, symbol_table*); // * - file, and the symbol_table
  ~lexer();
  bool is_success();
  int get_current_line();
  void fail();
  token* get_next_token();
  bool check_word(const char *);
  bool match(char);
  void collect_number();
  void collect_id();
  void collect_literal();
  void skip_line_comment();
  void skip_multline_comment();
  bool reset_lexeme();
  char next_character();
};


#endif
