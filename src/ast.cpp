#include "include/ast.h"

expression_node* create_primary_node(token literal)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::PRIMARY;
  init->data.primary.literal = literal;
  return init;
}

expression_node* create_binary_node(expression_node* left, token op, expression_node* right)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::BINARY;
  init->data.binary.right = right;
  init->data.binary.left = left;
  init->data.binary.op = op;
  return init;
}

expression_node* create_unary_node(token op, expression_node* right)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::UNARY;
  init->data.unary.op = op;
  init->data.unary.right = right;
  return init;
}

expression_node* create_grouping_node(expression_node* expr)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::GROUPING;
  init->data.grouping.expr = expr;
  return init;
}
