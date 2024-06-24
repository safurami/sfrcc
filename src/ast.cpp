#include "include/ast.h"
#include "include/my.h"

#include <cstdio>

void print_offset(int offset)
{
  for(int i = 0; i < offset;i++)
  {
    printf(" ");
  }
}

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

void free_expression_ast(expression_node* root)
{
  switch(root->tag)
  {
    case expression_node::PRIMARY:
      delete root;
      break;
    case expression_node::BINARY:
      free_expression_ast(root->data.binary.left);
      free_expression_ast(root->data.binary.right);
      delete root;
      break;
    case expression_node::UNARY:
      free_expression_ast(root->data.unary.right);
      delete root;
      break;
    case expression_node::GROUPING:
      free_expression_ast(root->data.grouping.expr);
      delete root;
      break;
    default:
      my_assert(false);
  }
}

void print_expression_ast(expression_node* root, int offset)
{
  switch(root->tag)
  {
    case expression_node::PRIMARY:
      print_offset(offset); printf("Node Type: Expression - Literal\n");
      print_offset(offset); printf("Token Type: %s\n", token_type2string(root->data.primary.literal.get_type()));
      print_offset(offset); printf("Attribute: %d\n", root->data.primary.literal.get_attribute());
      break;
    case expression_node::BINARY:
      print_offset(offset); printf("Node Type: Expression - Binary\n");
      print_offset(offset); printf("Operator: %s\n", token_type2string(root->data.binary.op.get_type()));
      print_offset(offset); printf("Left Node: {\n");
      print_expression_ast(root->data.binary.left, offset + OFFSET_INC);
      print_offset(offset); printf("}\n");
      print_offset(offset); printf("Right Node: {\n");
      print_expression_ast(root->data.binary.right, offset + OFFSET_INC);
      print_offset(offset); printf("}\n");
      break;
    case expression_node::UNARY:
      print_offset(offset); printf("Node Type: Expression - Unary\n");
      print_offset(offset); printf("Operator: %s\n", token_type2string(root->data.unary.op.get_type()));
      print_offset(offset); printf("Right Node: {\n");
      print_expression_ast(root->data.unary.right, offset + OFFSET_INC);
      print_offset(offset); printf("}\n");
      break;
    case expression_node::GROUPING:
      print_offset(offset); printf("Node Type: Expression - Grouping\n");
      print_offset(offset); printf("Expression: {\n");
      print_expression_ast(root->data.grouping.expr, offset + OFFSET_INC);
      print_offset(offset); printf("}\n");
      break;
    default:
      my_assert(false);
  }
}
