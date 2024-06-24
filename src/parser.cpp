#include "include/parser.h"
#include "include/ast.h"
#include "include/my.h"

parser::parser(lexer *lex, symbol_table *table)
{
  this->m_lexer = lex;
  this->m_table = table;
  this->m_was_error = false;
}

void parser::set_input(token *input)
{
  this->m_input = input;
}

token& parser::advance()
{
  if(this->m_input->get_type() != token_type::DOLLAR)
  {
    this->m_input++;
  }
  return this->previous();
}

bool parser::match(token_type type)
{
  if(this->check(type))
  {
    this->advance();
    return true;
  }
  return false;
}

bool parser::check(token_type type)
{
  return this->peek().get_type() == type;
}

token& parser::previous()
{
  return *(this->m_input - 1);
}

token& parser::peek()
{
  return *this->m_input;
}

bool parser::was_error()
{
  return this->m_was_error;
}

/*
 * Stuff for grammar.
 */

expression_node* parser::parse()
{
  return this->expression();
}

expression_node* parser::expression()
{
  return this->equality();
}

expression_node* parser::equality()
{
  expression_node* expr = this->comparison();
  if(expr == nullptr)
  {
    return nullptr;
  }
  while(this->match(token_type::NEQ) || this->match(token_type::IS_EQUAL))
  {
    token op = this->previous();
    expression_node* right = this->comparison();
    if(right == nullptr)
    {
      free_expression_ast(expr);
      return nullptr;
    }
    expr = create_binary_node(expr, op, right);
  }
  return expr;
}

expression_node* parser::comparison()
{
  expression_node* expr = this->term();
  if(expr == nullptr)
  {
    return nullptr;
  }
  while(this->match(token_type::GREATER) || this->match(token_type::GEQ) || this->match(token_type::LESS)
      || this->match(token_type::LEQ))
  {
    token op = this->previous();
    expression_node* right = this->term();
    if(right == nullptr)
    {
      free_expression_ast(expr);
      return nullptr;
    }
    expr = create_binary_node(expr, op, right);
  }
  return expr;
}

expression_node* parser::term()
{
  expression_node* expr = this->factor();
  if(expr == nullptr)
  {
    return nullptr;
  }
  while(this->match(token_type::MINUS) || this->match(token_type::PLUS))
  {
    token op = this->previous();
    expression_node* right = this->factor();
    if(right == nullptr)
    {
      free_expression_ast(expr);
      return nullptr;
    }
    expr = create_binary_node(expr, op, right);
  }
  return expr;
}

expression_node* parser::factor()
{
  expression_node* expr = this->unary();
  if(expr == nullptr)
  {
    return nullptr;
  }

  while(this->match(token_type::STAR) || this->match(token_type::DIVIDE))
  {
    token op = this->previous();
    expression_node* right = this->unary();
    if(right == nullptr)
    {
      free_expression_ast(expr);
      return nullptr;
    }
    expr = create_binary_node(expr, op, right);
  }
  return expr;
}

expression_node* parser::unary()
{
  if(this->match(token_type::MINUS) || this->match(token_type::BANG))
  {
    token op = this->previous();
    expression_node* expr = this->unary();
    if(expr == nullptr)
    {
      return nullptr;
    }
    return create_unary_node(op, expr);
  }
  return this->primary();
}

expression_node* parser::primary()
{
  if(this->match(token_type::NUMBER) || this->match(token_type::IDENTIFIER) || this->match(token_type::LITERAL))
  {
    return create_primary_node(this->previous());
  }
  // TODO: implement grouping.
  else
  {
    // TODO: implement error reporting.
    return nullptr;
  }
}
