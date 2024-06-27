#ifndef H_AST
#define H_AST

#include "token.h"

#define OFFSET_INC 3

void print_offset(int);

/*
 * Expression node structures.
 */

struct expression_node;

struct primary_node
{
  token literal;
  primary_node() {}
  primary_node(token tok): literal(tok) {}
};

struct binary_node
{
  expression_node* left;
  token op;
  expression_node* right;
  binary_node(): left(nullptr), right(nullptr) {}
  binary_node(expression_node* l, token oper, expression_node* r): left(l), op(oper), right(r) {}
};

struct unary_node
{
  token op;
  expression_node* right;
  unary_node(): right(nullptr) {}
  unary_node(token oper, expression_node* r): op(oper), right(r) {}
};

struct grouping_node
{
  expression_node* expr;
  grouping_node(): expr(nullptr) {}
  grouping_node(expression_node* node): expr(node) {}
};

struct expression_node
{
  enum
  {
    PRIMARY,
    BINARY,
    UNARY,
    GROUPING
  } tag;
  union data_t
  {
    primary_node primary;
    binary_node binary;
    unary_node unary;
    grouping_node grouping;
    data_t() {}
  } data;
};

expression_node* create_primary_node(token);
expression_node* create_binary_node(expression_node*, token, expression_node*);
expression_node* create_unary_node(token, expression_node*);
expression_node* create_grouping_node(expression_node*);

void free_expression_ast(expression_node*);
void print_expression_ast(expression_node*, int);


/*
 * Statement node structures.
 */

struct statement_node;

struct expr_stmt_node
{
  expression_node* expr;
  expr_stmt_node(): expr(nullptr) {}
  expr_stmt_node(expression_node* ex): expr(ex) {}
};


struct statement_node
{
  enum {
    EXPR_STMT,
  } tag;
  union data_t
  {
    expr_stmt_node expr_stmt;
    data_t() {}
  } data;
};

statement_node* create_expr_stmt_node(expression_node*);

void free_statement_ast(statement_node*);
void print_statement_ast(statement_node*, int);

#endif
