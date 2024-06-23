#ifndef H_PARSER
#define H_PARSER

#include "lexer.h"
#include "symbol_table.h"
#include "ast.h"
#include "my.h"

class parser
{
  bool m_was_error;
  lexer* m_lexer;
  symbol_table* m_table;
  token* m_input;
public:
  parser(lexer*, symbol_table*);
  void set_input(token*);
  bool was_error();
  bool match(token_type);
  bool check(token_type);
  token& peek();
  token& previous();
  token& advance();

};

#endif
