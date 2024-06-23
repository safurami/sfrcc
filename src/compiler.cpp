#include "include/compiler.h"

#include "include/my.h"

void compiler::free_all()
{
  if(this->m_table != nullptr)
  {
    delete this->m_table;
    this->m_table = nullptr;
  }
  if(this->m_lexer != nullptr)
  {
    delete this->m_lexer;
    this->m_lexer = nullptr;
  }
  if(this->m_parser != nullptr)
  {
    delete this->m_parser;
    this->m_parser = nullptr;
  }
}

compiler::compiler(const char *filename)
{
  this->m_table = new symbol_table();
  this->m_lexer = new lexer(filename, this->m_table);
  this->m_parser = new parser(this->m_lexer, this->m_table);

//  if(this->m_lexer->was_error()) // If failed to open the file.
// {
//    this->free_all();
//    return;
//  }


  if(!this->m_lexer->was_error())
  {
    this->compile();
  }
}

compiler::~compiler()
{
  this->free_all();
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

  if(this->m_lexer->was_error()) // If there was a bad token.
  {
    return;
  }
  this->m_table->dump_table();
  this->m_parser->set_input(vector.get_raw_pointer(0));

}
