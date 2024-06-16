#include "include/ast.h"


#include <cstdio>

ast_node* create_literal(token &tok)
{
  ast_node* init = new ast_node();
  init->tag = ast_node::LITERAL_NODE;
  init->data.literal.tok = tok;
  return init;
}

ast_node* create_unary(token &op, ast_node* expr)
{
  ast_node* init = new ast_node();
  init->tag = ast_node::UNARY_NODE;
  init->data.unary.op = op;
  init->data.unary.right = expr;
  return init;
}

ast_node* create_binary(ast_node* left, token &op, ast_node* right)
{
  ast_node* init = new ast_node();
  init->tag = ast_node::BINARY_NODE;
  init->data.binary.left = left;
  init->data.binary.right = right;
  init->data.binary.op = op;
  return init;
}

ast_node* create_grouping(ast_node* expr)
{
  ast_node* init = new ast_node();
  init->tag = ast_node::GROUPING_NODE;
  init->data.grouping.expr = expr;
  return init;
}

void free_ast(ast_node* root)
{
  if(root == nullptr)
  {
    return;
  }
  switch(root->tag)
  {
  case ast_node::LITERAL_NODE:
    delete root;
    break;
  case ast_node::BINARY_NODE:
    if(root->data.binary.left != nullptr)
    {
      free_ast(root->data.binary.left);
    }
    if(root->data.binary.right!= nullptr)
    {
      free_ast(root->data.binary.right);
    }
    delete root;
    break;
  case ast_node::UNARY_NODE:
    if(root->data.unary.right != nullptr)
    {
      free_ast(root->data.unary.right);
    }
    delete root;
    break;
  case ast_node::GROUPING_NODE:
    if(root->data.grouping.expr != nullptr)
    {
      free_ast(root->data.grouping.expr);
    }
    delete root;
    break;
  }
}

void print_ast(ast_node* root, int offset)
{
  switch(root->tag)
  {
  case ast_node::LITERAL_NODE:
    print_offset // idk how to make it else, i dont like it, but it is what it is
    printf("Node: Literal\n");
    print_offset
    printf("Token type: %s\n", token_type_to_string(root->data.literal.tok.get_type()));
    print_offset
    printf("Attribute: %d\n", root->data.literal.tok.get_attribute());
    break;
  case ast_node::BINARY_NODE:
    print_offset
    printf("Node: Binary\n");
    print_offset
    printf("Left node: {\n");
    print_ast(root->data.binary.left, offset + 3);
    print_offset
    printf("}\n");
    print_offset
    printf("Operator: %s\n", token_type_to_string(root->data.binary.op.get_type()));
    print_offset
    printf("Right node: {\n");
    print_ast(root->data.binary.right, offset + 3);
    print_offset
    printf("}\n");
    break;
  case ast_node::UNARY_NODE:
    print_offset
    printf("Node: Unary\n");
    print_offset
    printf("Operator: %s\n", token_type_to_string(root->data.unary.op.get_type()));
    print_offset
    printf("Right node: {\n");
    print_ast(root->data.unary.right, offset + 3);
    print_offset
    printf("}\n");
    break;
  case ast_node::GROUPING_NODE:
    print_offset
    printf("Node: Grouping\n");
    print_offset
    printf("Node: {\n");
    print_ast(root->data.grouping.expr, offset + 3);
    print_offset
    printf("}\n");
    break;
  }
}
