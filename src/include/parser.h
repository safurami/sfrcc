#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

class Parser
{
public:
  token *input;
  bool was_error;

  Parser(token *tokens_list);
  void next();
  bool match(token_type type);
  token *prev();

  ast_node *add_expression();
  ast_node *mul_expression();
  ast_node *primary();
};



#endif /* PARSER_H */
