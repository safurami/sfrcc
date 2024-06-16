#include "include/compiler.h"

#include "include/my.h"

compiler::compiler(const char *filename)
{
  this->m_table = new symbol_table();
  this->m_lexer = new lexer(filename, this->m_table);
  this->m_parser = new parser(this->m_lexer, this->m_table);

  if(this->m_lexer->was_error())
  {
    return;
  }

  token *tok;
  while((tok = this->m_lexer->get_next_token()), tok->get_type() != token_type::DOLLAR) { }
  this->m_table->dump_table();

  //this->compile();
}

compiler::~compiler()
{
  delete this->m_table;
  delete this->m_lexer;
  delete this->m_parser;
}

void compiler::compile()
{
  my::vector<token> vector(50);
  token *tok;
  do
  {
    tok = this->m_lexer->get_next_token();
    vector.push_back(*tok);
  } while(tok->get_type() != token_type::DOLLAR);

  if(this->m_lexer->was_error())
  {
    return;
  }

  this->m_table->dump_table();

  this->m_parser->set_input(vector.get_raw_pointer(0));

  ast_node* root = this->m_parser->parse();

  if(this->m_parser->was_error()) return;

  print_ast(root, 0);

  free_ast(root);

}
