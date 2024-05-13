/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

#include <iostream>

lexer::lexer(char *f) : table()
{
  this->file.open(f);
  if(!file.is_open())
  {
    this->buffer1 = nullptr; // Error, nothing is allocating
    this->buffer2 = nullptr;
    this->current_token = nullptr;
    return;
  }
  this->current_token = new token();
  this->buffer1 = new char[4096];
  this->buffer2 = new char[4096];
  this->file.read(this->buffer1, 4096);
  this->lexeme_begin = buffer1; // REMOVEME maybe dont need this
  this->forward = buffer1;
  this->current_line = 1;
}

bool lexer::is_success()
{
  if(this->buffer1 == nullptr)
    return false;
  return true;
}

lexer::~lexer()
{
  if(this->file.is_open())
  {
    this->file.close();
  }
  delete this->current_token;
  delete[] this->buffer1;
  delete[] this->buffer2;
}

token* lexer::get_next_token()
{

  switch(*forward++)
  {
    // TODO
  }

  return this->current_token;
}
