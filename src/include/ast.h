#ifndef H_AST
#define H_AST

#include "token.h"


#define print_offset \
for(int i = 0; i < offset; i++) \
{ \
printf(" "); \
}

struct ast_node;

struct unary_node
{
  token op;
  ast_node* right;
  unary_node(): right(nullptr) {}
  unary_node(token op, ast_node* right)
  {
    this->op = op;
    this->right = right;
  }
};

struct binary_node
{
  ast_node* left;
  token op;
  ast_node* right;
  binary_node(): left(nullptr), right(nullptr) {}
  binary_node(ast_node* left, token op, ast_node* right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }
};

struct literal_node
{
  token tok;
  literal_node() {}
  literal_node(token tok)
  {
    this->tok = tok;
  }
};

struct grouping_node
{
  ast_node* expr;
  grouping_node(): expr(nullptr) {}
  grouping_node(ast_node* expr)
  {
    this->expr = expr;
  }
};

struct ast_node
{
  enum
  {
    BINARY_NODE,
    UNARY_NODE,
    LITERAL_NODE,
    GROUPING_NODE
  } tag;
  union data_t
  {
    unary_node unary;
    binary_node binary;
    literal_node literal;
    grouping_node grouping;

    data_t() {}
    ~data_t() {}
  } data;
};

ast_node* create_literal(token&);
ast_node* create_unary(token&, ast_node*);
ast_node* create_binary(ast_node*, token&, ast_node*);
ast_node* create_grouping(ast_node*);
void free_ast(ast_node*);

void print_ast(ast_node*, int); // Just for debug

#endif
