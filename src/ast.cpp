#include <new>
#include <cstdio>
#include <cassert>

#include "include/ast.h"


ast_node *make_literal_node(int intval)
{
  ast_node *init = new(std::nothrow) ast_node();
  if(init == nullptr)
  {
    printf("Bad allocation in make_literal_node()\n");

    // TOOD: maybe in future remove it with handling, not just assert.
    assert(false);
  }
  init->tag = ast_node::LITERAL;
  init->data.literal.intval = intval;
  return init;
}

/*ast_node *make_unary_node(token_type op, ast_node *expr)
{
  ast_node *init = new(std::nothrow) ast_node();
  if(init == nullptr)
  {
    printf("Bad allocation in make_unary_node()\n");

    // TOOD: maybe in future remove it with handling, not just assert.
    assert(false);
  }
  init->tag = ast_node::UNARY;
  init->data.unary.op = op;
  init->data.unary.expr = expr;
  return init;
}*/

ast_node *make_binary_node(ast_node *left, token_type op, ast_node *right)
{
  ast_node *init = new(std::nothrow) ast_node();
  if(init == nullptr)
  {
    printf("Bad allocation in make_binary_node()\n");

    // TOOD: maybe in future remove it with handling, not just assert.
    assert(false);
  }
  init->tag = ast_node::BINARY;
  init->data.binary.left = left;
  init->data.binary.op = op;
  init->data.binary.right = right;
  return init;
}

void free_ast(ast_node *root)
{
  switch(root->tag)
  {
    case ast_node::LITERAL:
      delete root;
      break;
/*
      case ast_node::UNARY:
      free_ast(root->data.unary.expr);
      delete root;
      break;
*/
    case ast_node::BINARY:
      free_ast(root->data.binary.left);
      free_ast(root->data.binary.right);
      delete root;
      break;
    default:
      assert(false);
  }
}

int interpret_ast(ast_node *root)
{
  int leftval, rightval;
  leftval = rightval = 0;
  switch(root->tag)
  {
    case ast_node::LITERAL:
      return root->data.literal.intval;
/*
    case ast_node::UNARY:
      leftval = interpret_ast(root->data.unary.expr);
      switch(root->data.unary.op)
      {
        case token_type::MINUS:
          return -leftval;
        default:
          assert(false);
      }
      break;
*/
    case ast_node::BINARY:
      leftval = interpret_ast(root->data.binary.left);
      rightval = interpret_ast(root->data.binary.right);
      switch(root->data.binary.op)
      {
        case token_type::MINUS:
          return leftval - rightval;
        case token_type::PLUS:
          return leftval + rightval;
        case token_type::STAR:
          return leftval * rightval;
        case token_type::SLASH:
          return leftval / rightval;
        default:
          assert(false);
      }
    default:
      assert(false);
  }
}

void print_offset(int offset)
{
  for(int i = 0; i < offset; i++)
  {
    printf(" ");
  }
}

void print_ast(ast_node *root, int offset)
{
  switch(root->tag)
  {
    case ast_node::LITERAL:
      print_offset(offset); printf("Node Type: LITERAL\n");
      print_offset(offset); printf("Value: %d\n", root->data.literal.intval);
      break;
/*
    case ast_node::UNARY:
      print_offset(offset); printf("Node Type: UNARY\n");
      print_offset(offset); printf("Operator: %s\n", tok2string(root->data.unary.op));
      print_offset(offset); printf("Expr {\n");
      print_ast(root->data.unary.expr, offset + PRINT_OFFSET);
      print_offset(offset); printf("}\n");
      break;
*/
    case ast_node::BINARY:
      print_offset(offset); printf("Node Type: BINARY\n");
      print_offset(offset); printf("Operator: %s\n", tok2string(root->data.binary.op));
      print_offset(offset); printf("Left Expr {\n");
      print_ast(root->data.binary.left, offset + PRINT_OFFSET);
      print_offset(offset); printf("}\n");
      print_offset(offset); printf("Right Expr {\n");
      print_ast(root->data.binary.right, offset + PRINT_OFFSET);
      print_offset(offset); printf("}\n");
      break;
    default:
      assert(false);
  }
}
