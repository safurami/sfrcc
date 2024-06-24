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

  void report_error(const char*);
  bool consume(token, const char*);

  expression_node* parse();


  expression_node* expression();
  expression_node* equality();
  expression_node* comparison();
  expression_node* term();
  expression_node* factor();
  expression_node* unary();
  expression_node* primary();

};

#endif
