#ifndef H_COMPILER
#define H_COMPILER

#include "lexer.h"
#include "symbol_table.h"
#include "parser.h"

class compiler
{
  lexer* m_lexer;
  symbol_table* m_table;
  parser* m_parser;
public:
  compiler(const char *);
  ~compiler();
  void compile();
};

#endif
