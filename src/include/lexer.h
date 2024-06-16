#ifndef H_LEXER
#define H_LEXER

#include "token.h"
#include "symbol_table.h"

#include <fstream>

class lexer
{
private:
  int m_current_line;
  char* m_buffer;
  char* m_input; // pointer to read lexemes
  char* m_lexeme_start;
  token* m_current_token;
  std::ifstream m_file;
  bool m_was_error;

  // Fileds that belongs not to only lexer.
  symbol_table* m_table;

public:
  lexer(lexer&) = delete;
  lexer(const char *, symbol_table*);
  ~lexer();
  bool was_error();
  char peek();
  char advance();
  bool match(char);
  void collect_id();
  int collect_literal();
  void report_error();
  void report_error(int, const char*);
  void collect_number();
  bool check_rest(const char *);

  token* get_next_token();
  void skip_line_comment();
  void skip_multline_comment();

  bool reset_lexeme();
  void free_all();
};


#endif
