#ifndef H_PARSER
#define H_PARSER

#include "lexer.h"
#include "symbol_table.h"
#include "ast.h"

class parser
{
  bool m_was_error;
  lexer* m_lexer;
  symbol_table* m_table;
  token* m_input;
public:
  parser(lexer*, symbol_table*);
  ast_node* parse();
  void set_input(token*);
  bool was_error();

  bool match(token_type);
  bool check(token_type);
  token& peek();
  token& previous();
  token& advance();
  void consume(token_type, const char*);
  void error(token, const char*);


  // grammar
  ast_node* expression();
  ast_node* equality();
  ast_node* comparison();
  ast_node* term();
  ast_node* factor();
  ast_node* unary();
  ast_node* primary();
};

#endif
