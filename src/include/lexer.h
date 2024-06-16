#ifndef H_LEXER
#define H_LEXER

#include "token.h"
#include "symbol_table.h"

#include <fstream>

class lexer
{
private:
  int m_current_line;
  char* m_buffer1;
  char* m_buffer2;
  char* m_lexeme_begin; // pointer to point to start of the current lexeme
  char* m_forward; // pointer to read lexemes
  token* m_current_token;
  std::ifstream m_file;
  bool m_was_error;

  // Fileds that belongs not to only lexer.
  symbol_table* m_table;

public:
  lexer(lexer&) = delete;
  lexer(const char *, symbol_table*); // * - file, and the symbol_table
  ~lexer();
  bool is_success();
  int get_current_line();
  void lexer_error();
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
  void free_all();
  char peek();
  bool was_error();
};


#endif
