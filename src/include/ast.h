#ifndef AST_H
#define AST_H

#include "token.h"

// Offset in spaces to indent each node of AST.
#define PRINT_OFFSET 3

// Just ast node, can contain any node structure.
struct ast_node;

struct binary_node
{
  ast_node *left;
  ast_node *right;
  token_type op;
};

struct unary_node
{
  token_type op;
  ast_node *expr;
};

struct literal_node
{
  int intval;
};

// Struct for ast node.
struct ast_node
{
  enum
  {
    BINARY,
    LITERAL
  } tag;
  union data_u
  {
    binary_node binary;
    literal_node literal;
    data_u() {}
  } data;

};

ast_node *make_literal_node(int intval);
ast_node *make_unary_node(token_type op, ast_node *expr);
ast_node *make_binary_node(ast_node *left, token_type op, ast_node *right);

void free_ast(ast_node *root);
int interpret_ast(ast_node *root);


void print_offset(int offset);
void print_ast(ast_node *root, int offset);

#endif /* AST_H */
