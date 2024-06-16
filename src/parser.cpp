#include "include/parser.h"
#include "include/ast.h"

/*
  expression     → equality ;
  equality       → comparison ( ( "!=" | "==" ) comparison )* ;
  comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
  term           → factor ( ( "-" | "+" ) factor )* ;
  factor         → unary ( ( "/" | "*" ) unary )* ;
  unary          → ( "!" | "-" ) unary
                   | primary ;
  primary        → NUMBER | STRING | "true" | "false" | "nil"
                 | "(" expression ")" ;
*/

parser::parser(lexer *lex, symbol_table *table)
{
  this->m_lexer = lex;
  this->m_table = table;
}

ast_node* parser::parse()
{
  ast_node* root = this->expression();
  if(!this->check(token_type::DOLLAR))
  {
    this->error(this->peek(), "");
  }
  if(this->m_was_error)
  {
    free_ast(root);
    return nullptr;
  }
  return root;
}

ast_node* parser::expression()
{
  return this->equality();
}

ast_node* parser::equality()
{
  ast_node* expr = comparison();
  while(this->match(token_type::NEQ) || this->match(token_type::IS_EQUAL))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->comparison());
  }
  return expr;

}

ast_node* parser::comparison()
{
  ast_node* expr = term();
  while(this->match(token_type::GREATER)
         || this->match(token_type::GEQ)
         || this->match(token_type::LESS)
         || this->match(token_type::LEQ))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->term());
  }
  return expr;
}

ast_node* parser::term()
{
  ast_node* expr = factor();
  while(this->match(token_type::PLUS) || this->match(token_type::MINUS))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->factor());
  }
  return expr;
}

ast_node* parser::factor()
{
  ast_node* expr = unary();
  while(this->match(token_type::DIVIDE) || this->match(token_type::STAR))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->unary());
  }
  return expr;
}

ast_node* parser::unary()
{
  if(this->match(token_type::BANG) || this->match(token_type::MINUS))
  {
    token op = this->previous();
    ast_node* expr = create_unary(op, this->unary());
    return expr;
  }
  return this->primary();
}

ast_node* parser::primary()
{
  if(this->match(token_type::NUMBER))
  {
    ast_node* literal= create_literal(this->previous());
    return literal;
  }
  else if(this->match(token_type::OPEN_PAREN))
  {
    ast_node* expr = this->expression();
    this->consume(token_type::CLOSE_PAREN, "Expect ')'");
    return create_grouping(expr);
  }
  error(this->peek(), "Expect expression");
  return nullptr;
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

void parser::consume(token_type type, const char* expect)
{
  if(this->check(type))
  {
    this->advance();
    return;
  }
  this->error(this->peek(), expect);
}

void parser::error(token error_place, const char *expect)
{
  this->m_was_error = true;
  if(error_place.get_type() != token_type::DOLLAR)
  {
    printf("[Line %d] Unexpected token '%s', %s.\n", error_place.get_line(), token_type_to_string(error_place.get_type()), expect);
  }
  else
  {
    printf("[At The End] Unexpected token '%s', %s.\n", token_type_to_string(error_place.get_type()), expect);
  }
}

bool parser::was_error()
{
  return this->m_was_error;
}
