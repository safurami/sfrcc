#include "include/parser.h"
#include "include/ast.h"
#include "include/my.h"

parser::parser(lexer *lex, symbol_table *table)
{
  this->m_lexer = lex;
  this->m_table = table;
  this->m_was_error = false;
}

my::vector<statement_node*>* parser::parse()
{
  return this->program();
}

expression_node* parser::expression()
{
  return this->equality();
}

expression_node* parser::equality()
{
  expression_node* expr = comparison();
  while(this->match(token_type::NEQ) || this->match(token_type::IS_EQUAL))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->comparison());
  }
  return expr;

}

expression_node* parser::comparison()
{
  expression_node* expr = term();
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

expression_node* parser::term()
{
  expression_node* expr = factor();
  while(this->match(token_type::PLUS) || this->match(token_type::MINUS))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->factor());
  }
  return expr;
}

expression_node* parser::factor()
{
  expression_node* expr = unary();
  while(this->match(token_type::DIVIDE) || this->match(token_type::STAR))
  {
    token op = this->previous();
    expr = create_binary(expr, op, this->unary());
  }
  return expr;
}

expression_node* parser::unary()
{
  if(this->match(token_type::BANG) || this->match(token_type::MINUS))
  {
    token op = this->previous();
    expression_node* expr = create_unary(op, this->unary());
    return expr;
  }
  return this->primary();
}

expression_node* parser::primary()
{
  if(this->match(token_type::NUMBER) || this->match(token_type::LITERAL) || this->match(token_type::IDENTIFIER))
  {
    expression_node* literal = create_literal(this->previous());
    return literal;
  }
  else if(this->match(token_type::OPEN_PAREN))
  {
    expression_node* expr = this->expression();
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

my::vector<statement_node*>* parser::program()
{
  my::vector<statement_node*>* vec = new my::vector<statement_node*>(20);
  statement_node* stmt;
  while(!this->check(token_type::DOLLAR))
  {
    if(this->was_error())
    {
      break;
    }
    stmt = this->declaration();
    vec->push_back(stmt);
  }
  this->consume(token_type::DOLLAR, "Expected End Of File");
  if(this->was_error())
  {
    statement_node** pointer = vec->get_raw_pointer(0); // Freeing all previous statement nodes, and returning nullptr;
    for(int i = 0; i < vec->get_size(); i++)
    {
      free_stmt_ast(pointer[i]);
    }
    delete vec; // If error, freeing the vector.
    return nullptr;
  }

  return vec;
}

statement_node* parser::declaration()
{
  if(this->match(token_type::INT) || this->match(token_type::CHAR) || this->match(token_type::VOID))
  {
    token type = this->previous();
    this->consume(token_type::IDENTIFIER, "Expected identifier");
    token id = this->previous();
    this->consume(token_type::ASSIGN, "Expected assign");
  }
  return this->statement();
}

statement_node* parser::statement()
{
  expression_node* expr = this->expression();
  statement_node* statement_expr_node = create_expr_stmt(expr);
  this->consume(token_type::SEMICOLON, "Expected semicolon after expresison");
  return statement_expr_node;
}
