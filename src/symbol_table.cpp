#include "include/symbol_table.h"
#include "include/my.h"

#include <iostream> // REMOVEME

symbol_table::node::node(node &other)
{
  int size = my::strlen(other.name);
  this->name = new char[size + 1];
  my::strncpy(this->name, other.name, size);
  this->size = other.size;
  this->line_of_declaration = other.line_of_declaration;
  this->address = other.address;
}

symbol_table::node::node()
{
  std::cout << "constructor" << std::endl; // REMOVEME
  this->name = nullptr;
}

symbol_table::node::~node()
{
  if(this->name != nullptr)
  {
    delete[] this->name;
  }
}

const char* symbol_table::node::get_name()
{
  return this->name;
}

void symbol_table::node::set_name(const char *name)
{
  int size = my::strlen(name);
  this->name = new char[size + 1];
  my::strncpy(this->name, name, size);
}

void symbol_table::node::set_size(int size)
{
  this->size = size;
}

int symbol_table::node::get_size()
{
  return this->size;
}

void symbol_table::node::set_line(int line)
{
  this->line_of_declaration = line;
}

int symbol_table::node::get_line()
{
  return this->line_of_declaration;
}

void symbol_table::node::set_address(int address)
{
  this->address = address;
}

int symbol_table::node::get_address()
{
  return this->address;
}

symbol_table::symbol_table() : table(100) {}

/*
 * Takes the lexeme, if its not yet in symbol table -
 * writes it to symbol table and return index.
 * If it is already there, return index
 */
int symbol_table::install_id(const char *start, const char *end)
{
  // TODO
  return 0;
}

/*
 * Computing hash of lexeme and % by size of table(vector table.size)
 */
int symbol_table::hash(const char *start, const char *end)
{
  // TODO
  return 0;
}
