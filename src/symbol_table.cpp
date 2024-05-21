#include "include/symbol_table.h"
#include "include/my.h"

#include <cstdio>

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
  if(this->name != nullptr) { delete[] this->name; }
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

symbol_table::symbol_table()
{
  for(int i = 0; i < 100; i++) { this->table[i] = nullptr; }
}

symbol_table::~symbol_table()
{
  for(int i = 0; i < 100; i++) // HASH
  {
    if(this->table[i] != nullptr) { delete this->table[i]; }
  }
}

/*
 * Takes the lexeme, if its not yet in symbol table -
 * writes it to symbol table and return index.
 * If it is already there, return index
 */
int symbol_table::install_id(const char *start, const char *end) // TODO: refactor this
{
  unsigned int sum = this->hash(start, end);

  char *tmp = new char[end - start + 2]; // TODO: platform depend(MAYBE, idk exactly), fix it
  my::strncpy(tmp, start, end - start + 1);

  if(this->get_node(sum) != nullptr && my::strcmp(tmp, this->get_node(sum)->get_name()))
  {
    delete[] tmp;
    return sum;
  }
  // TODO: implement returning index of elements that stored in another index, because previous index was occupied.
  for(;this->get_node(sum) != nullptr; sum++) {}
  this->table[sum] = new node();
  this->get_node(sum)->set_name(tmp);
  delete[] tmp;
  return sum;
}

// TODO: implement normal hash function, not this
unsigned int symbol_table::hash(const char *start, const char *end)
{
  unsigned int sum = 0;
  do
  {
    sum += *start * 31 + 23;
    start++;
  } while(start != end + 1);
  sum = sum % 100; // HASH
  return sum;
}

symbol_table::node* symbol_table::get_node(int index)
{
  if(index < 0 || index > 99) // invalid index, returning first element, HASH
  {
    return this->table[0];
  }
  return this->table[index];
}

void symbol_table::dump_table() // TODO: maybe remove in future.
{
  printf("\n---Table Dump---\n");
  for(int i = 0; i < 100; i++) // HASH
  {
    if(this->get_node(i) != nullptr)
    {
      printf("Index: %d\tName:%s\n", i, this->get_node(i)->get_name());
    }
  }
  printf("---End of Dump---\n");
}
