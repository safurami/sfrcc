#include "include/ast.h"
#include "include/my.h"

#include <cstdio>

expression_node* create_literal(token &tok)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::LITERAL_NODE;
  init->data.literal.tok = tok;
  return init;
}

expression_node* create_unary(token &op, expression_node* expr)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::UNARY_NODE;
  init->data.unary.op = op;
  init->data.unary.right = expr;
  return init;
}

expression_node* create_binary(expression_node* left, token &op, expression_node* right)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::BINARY_NODE;
  init->data.binary.left = left;
  init->data.binary.right = right;
  init->data.binary.op = op;
  return init;
}

expression_node* create_grouping(expression_node* expr)
{
  expression_node* init = new expression_node();
  init->tag = expression_node::GROUPING_NODE;
  init->data.grouping.expr = expr;
  return init;
}

void free_expr_ast(expression_node* root)
{
  if(root == nullptr)
  {
    return;
  }
  switch(root->tag)
  {
  case expression_node::LITERAL_NODE:
    delete root;
    break;
  case expression_node::BINARY_NODE:
    if(root->data.binary.left != nullptr)
    {
      free_expr_ast(root->data.binary.left);
    }
    if(root->data.binary.right!= nullptr)
    {
      free_expr_ast(root->data.binary.right);
    }
    delete root;
    break;
  case expression_node::UNARY_NODE:
    if(root->data.unary.right != nullptr)
    {
      free_expr_ast(root->data.unary.right);
    }
    delete root;
    break;
  case expression_node::GROUPING_NODE:
    if(root->data.grouping.expr != nullptr)
    {
      free_expr_ast(root->data.grouping.expr);
    }
    delete root;
    break;
  default:
    my_assert(false);
  }
}

void free_stmt_ast(statement_node* root)
{
  switch(root->tag)
  {
    case statement_node::EXPRESSION_STMT:
      if(root->data.expr_stmt.expr != nullptr)
      {
        free_expr_ast(root->data.expr_stmt.expr);
      }
      delete root;
      break;
    default:
      my_assert(false);
  }

}

void print_offset(int offset)
{
  for(int i = 0; i < offset; i++)
  {
    printf(" ");
  }
}

void print_stmt_ast(statement_node* root, int offset)
{
  print_offset(offset);

  switch(root->tag)
  {
    case statement_node::EXPRESSION_STMT:
      printf("Node Type: Expression Statement Node\n");
      print_offset(offset);
      printf("Expression: {\n");
      print_expr_ast(root->data.expr_stmt.expr, offset + PRINTING_OFFSET);
      print_offset(offset);
      printf("}\n");
      break;
    default:
      my_assert(false);
  }
}

void print_expr_ast(expression_node* root, int offset)
{
  print_offset(offset);

  switch(root->tag)
  {
    case expression_node::LITERAL_NODE:
      printf("Node Type: Literal Node\n");
      print_offset(offset);
      printf("Data Type: %s\n", token_type_to_string(root->data.literal.tok.get_type()));
      print_offset(offset);
      printf("Attribute: %d\n", root->data.literal.tok.get_attribute());
      break;
    case expression_node::UNARY_NODE:
      printf("Node Type: Unary Node\n");
      print_offset(offset);
      printf("Operator: %s\n", token_type_to_string(root->data.unary.op.get_type()));
      print_offset(offset);
      printf("Right Node: {\n");
      print_expr_ast(root->data.unary.right, offset + PRINTING_OFFSET);
      print_offset(offset);
      printf("}\n");
      break;
    case expression_node::BINARY_NODE:
      printf("Node Type: Binary Node\n");
      print_offset(offset);
      printf("Operator: %s\n", token_type_to_string(root->data.binary.op.get_type()));
      print_offset(offset);
      printf("Left Node: {\n");
      print_expr_ast(root->data.binary.left, offset + PRINTING_OFFSET);
      print_offset(offset);
      printf("}\n");
      print_offset(offset);
      printf("Right Node: {\n");
      print_expr_ast(root->data.binary.right, offset + PRINTING_OFFSET);
      print_offset(offset);
      printf("}\n");
      break;
    case expression_node::GROUPING_NODE:
      printf("Node Type: Grouping Node\n");
      print_offset(offset);
      printf("Expression: {\n");
      print_expr_ast(root->data.grouping.expr, offset + PRINTING_OFFSET);
      print_offset(offset);
      printf("}\n");
      break;
    default:
      my_assert(false);
  }
}

statement_node* create_expr_stmt(expression_node* expr)
{
  statement_node* init = new statement_node();
  init->tag = statement_node::EXPRESSION_STMT;
  init->data.expr_stmt.expr = expr;
  return init;
}
