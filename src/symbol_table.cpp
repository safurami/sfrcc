#include "include/symbol_table.h"
#include "include/my.h"

#include <cstdio>

symbol_table::node::node()
{
  this->m_name = nullptr;
}

symbol_table::node::~node()
{
  if(this->m_name != nullptr)
  {
    delete[] this->m_name;
  }
}

const char* symbol_table::node::get_name()
{
  return this->m_name;
}

void symbol_table::node::set_name(const char *name)
{
  if(this->m_name != nullptr) { delete[] this->m_name; }
  int size = my::strlen(name);
  this->m_name = new char[size + 1];
  my::strncpy(this->m_name, name, size);
}

void symbol_table::node::set_size(int size)
{
  this->m_size = size;
}

int symbol_table::node::get_size()
{
  return this->m_size;
}

void symbol_table::node::set_line(int line)
{
  this->m_line_of_declaration = line;
}

int symbol_table::node::get_line()
{
  return this->m_line_of_declaration;
}

void symbol_table::node::set_address(int address)
{
  this->m_address = address;
}

int symbol_table::node::get_address()
{
  return this->m_address;
}

void symbol_table::node::set_type(node_type type)
{
  this->m_type = type;
}

node_type symbol_table::node::get_type()
{
  return this->m_type;
}

symbol_table::symbol_table()
{
  for(int i = 0; i < SYM_TBL_SIZE; i++) { this->table[i] = nullptr; } // HASH
}

symbol_table::~symbol_table()
{
  for(int i = 0; i < SYM_TBL_SIZE; i++) // HASH
  {
    if(this->table[i] != nullptr) { delete this->table[i]; }
  }
}

/*
 * Takes the lexeme, if its not yet in symbol table -
 * writes it to symbol table and return index.
 * If it is already there, return index
 */
int symbol_table::install_node(const char *start, const char *end, node_type type) // TODO: refactor this
{
  unsigned int sum = this->hash(start, end);

  char *tmp = new char[end - start + 2]; // Alocating memory for lexeme
  my::strncpy(tmp, start, end - start + 1);

#ifdef DEBUG
  printf("[Symbol Table] Called function to register '%s' node...\n", tmp);
#endif

  if(this->get_node(sum) != nullptr && my::strcmp(tmp, this->get_node(sum)->get_name()))
  {
#ifdef DEBUG
    printf("[Symbol Table] '%s' is already in the table\n", tmp);
#endif
    delete[] tmp;
    return sum;
  }
  // TODO: implement returning index of elements that stored in another index, because previous index was occupied.
  for(;this->get_node(sum) != nullptr; sum++) {}
  this->table[sum] = new node();
  this->get_node(sum)->set_name(tmp);
  this->get_node(sum)->set_type(type);
#ifdef DEBUG
  printf("[Symbol Table] Node '%s' was add to the table\n", tmp);
#endif
  delete[] tmp;
  return sum;
}

// TODO: What the hell is that XDDDDD.
unsigned int symbol_table::hash(const char *start, const char *end)
{
  unsigned int sum = 0;
  do
  {
    sum += *start * 31 + 23;
    start++;
  } while(start != end + 1);
  sum = sum % SYM_TBL_SIZE; // HASH
  return sum;
}

symbol_table::node* symbol_table::get_node(int index)
{
  if(index < 0 || index > SYM_TBL_SIZE - 1) // invalid index, returning first element, HASH
  {
    return this->table[0];
  }
  return this->table[index];
}

void symbol_table::dump_table()
{
  printf("\n---Table Dump---\n\n");
  for(int i = 0; i < SYM_TBL_SIZE; i++) // HASH
  {
    if(this->get_node(i) != nullptr)
    {
      printf("Index: %02d | Name:%10s | Type: ", i, this->get_node(i)->get_name());
      switch(this->get_node(i)->get_type())
      {
      case node_type::NUMBER: printf("Number"); break;
      case node_type::IDENTIFIER: printf("Identifier"); break;
      case node_type::LITERAL: printf("Literal"); break;
      }
      printf("\n");
    }
  }
  printf("\n---End of Dump---\n\n");
}
