/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

#include <iostream>

lexer_handler::lexer_handler(char *f)
{
  this->file.open(f);
  if(!file.is_open())
  {
    this->current_token = nullptr; // TODO maybe throw instead
    return;
  }
  this->storage= new char[4096];
  my::fgets(storage, this->file);
  this->input = storage;
  this->current_line = 1;
  this->current_token = new token();
}

lexer_handler::~lexer_handler()
{
  this->file.close();
  delete this->current_token;
  this->current_token = nullptr;
  delete[] this->storage;
  this->storage = nullptr;
  this->input= nullptr;
}

token* lexer_handler::get_next_token()
{
  if(*this->input == '\0')
  {
    if(my::fgets(this->storage, this->file) == EOF)
    {
      this->current_token->set_type(DOLLAR);
      return this->current_token;
    }
    this->input = this->storage;
    this->current_line++;
  }

  for(;*this->input == ' ' && *this->input == '\t';this->input++) {}


  return this->current_token;
}
