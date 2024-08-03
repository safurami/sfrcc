#include <cstdio>

#include "include/parser.h"


Parser::Parser(token *tokens_list): input(tokens_list), was_error(false) {}

// Method for increasing pointer that points to input.
// If EOF, then nothing was increased.
void Parser::next()
{
  if(this->input->type == token_type::END)
  {
    return;
  }
  this->input++;
}

// Method for conditional increase pointer that points to input.
// Returns true if types were matched and pointer increased,
// otherwise returns false.
bool Parser::match(token_type type)
{
  if(this->input->type == type)
  {
    this->next();
    return true;
  }
  return false;
}

// Returns previous tokens that was read.
token *Parser::prev()
{
  return this->input - 1;
}

ast_node *Parser::add_expression()
{
  ast_node *expr = this->mul_expression();
  if(expr == nullptr)
  {
    return nullptr;
  }
  while(this->match(token_type::PLUS) || this->match(token_type::MINUS))
  {
    token_type op = this->prev()->type;
    ast_node *right = this->mul_expression();
    if(right == nullptr) // maybe also add check for this->was_error
    {
      free_ast(expr);
      return nullptr;
    }
    expr = make_binary_node(expr, op, right);
  }
  return expr;
}



ast_node *Parser::mul_expression()
{
  ast_node *expr = this->primary();

  // Null pointer means that parse failed.
  if(expr == nullptr)
  {
    return nullptr;
  }
  while(this->match(token_type::STAR) || this->match(token_type::SLASH))
  {
    token_type op = this->prev()->type;
    ast_node *right = this->primary();

    if(right == nullptr)
    {
      free_ast(expr);
      return nullptr;
    }

    expr = make_binary_node(expr, op, right);
  }
  return expr;
}

// Primary level of expression.
ast_node *Parser::primary()
{
  if(this->match(token_type::INTLIT))
  {
    return make_literal_node(this->prev()->intval);
  }
  else
  {
    printf("Expected Int Literal, got %s\n", tok2string(this->input->type));
    this->was_error = true;
  }
  return nullptr;
}
