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
 * Return true if token was consumed.
 * If token was not consumed, calls r*eport_error and return false.
 */
bool parser::consume(token_type type, const char* message)
{
  if(!this->match(type))
  {
    this->report_error(message);
    return false;
  }
  return true;
}

void parser::report_error(const char *message)
{
  this->m_was_error = true;
  printf("[Line %d] Unexpected token %s, %s\n", this->peek().get_line(), token_type2string(this->peek().get_type()), message);
}

void parser::free_statements(my::vector<statement_node*>* vector)
{
  int size = vector->get_size();
  for(int i = 0; i < size; i++)
  {
    free_statement_ast(vector->get(i));
  }
}

/*
 * Stuff for grammar.
 */

my::vector<statement_node*>* parser::parse()
{
  return this->program();
}

my::vector<statement_node*>* parser::program()
{
  auto statements_list = new my::vector<statement_node*>(20);
  while(this->peek().get_type() != token_type::DOLLAR && !this->was_error()) // TODO: !this->was_error() is not necessery i think.
  {
    statement_node* node = this->statement(); // TODO: possible bug in future. Statement can return nullptr.
    if(node == nullptr)
    {
      break;
    }
    statements_list->push_back(node);
  }
  if(!this->was_error()) // To avoid an error when there was already an error in the middle.
  {
    this->consume(token_type::DOLLAR, "Expected End Of File");
  }
  if(this->was_error())
  {
    this->free_statements(statements_list);
    delete statements_list;
    return nullptr;
  }
  return statements_list;
}

statement_node* parser::statement() // I guess in this method i should implement synchronization.
{
  token* saved_ptr = this->m_input; // Save pointer, to try every production.

  // expr_stmt
  { 
    expression_node* expr = this->expression();
    if(expr == nullptr)
    {
      return nullptr; // TODO: remake to make it possible to try another production.
    }
    if(!this->consume(token_type::SEMICOLON, "Expected semicolon after expression"))
    {
      free_expression_ast(expr);
      return nullptr;
    }
    return create_expr_stmt_node(expr);
  }
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
  else if(this->match(token_type::OPEN_PAREN))
  {
    expression_node* expr = this->expression();
    if(expr == nullptr)
    {
      return nullptr;
    }
    if(!this->consume(token_type::CLOSE_PAREN, "Expected ')' after expression"))
    {
      free_expression_ast(expr);
      return nullptr;
    }
    return create_grouping_node(expr);
  }
  else
  {
    this->report_error("Expected expression");
    return nullptr;
  }
}
