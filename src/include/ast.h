#ifndef H_AST
#define H_AST

#include "token.h"

#define PRINTING_OFFSET 3

struct expression_node;

struct unary_node
{
  token op;
  expression_node* right;
  unary_node(): right(nullptr) {}
  unary_node(token op, expression_node* right)
  {
    this->op = op;
    this->right = right;
  }
};

struct binary_node
{
  expression_node* left;
  token op;
  expression_node* right;
  binary_node(): left(nullptr), right(nullptr) {}
  binary_node(expression_node* left, token op, expression_node* right)
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
  expression_node* expr;
  grouping_node(): expr(nullptr) {}
  grouping_node(expression_node* expr)
  {
    this->expr = expr;
  }
};

struct expression_node
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

expression_node* create_literal(token&);
expression_node* create_unary(token&, expression_node*);
expression_node* create_binary(expression_node*, token&, expression_node*);
expression_node* create_grouping(expression_node*);

// ------ //

struct expression_statement_node
{
  expression_node* expression;
  expression_statement_node(): expression(nullptr) {}
  expression_statement_node(expression_node* expr): expression(expr) {}
};

struct var_declaration_node
{
  token type;
  token name;
  expression_node* init_val;
}

struct statement_node
{
  enum
  {
    EXPRESSION_STMT,
  } tag;
  union data_t
  {
    struct { expression_node* expr; } expr_stmt;

    data_t() {}
  } data;
};

statement_node* create_expr_stmt(expression_node*);

void free_expr_ast(expression_node*);
void free_stmt_ast(statement_node*);

void print_stmt_ast(statement_node*, int);
void print_expr_ast(expression_node*, int);
void print_offset(int);

void print_ast(expression_node*, int); // Just for debug // TODO mofidy to work with statements

#endif
