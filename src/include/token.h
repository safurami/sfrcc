#ifndef TOKEN_H
#define TOKEN_H

#include <vector>

enum class token_type {
  PLUS, MINUS, STAR, SLASH, SEMICOLON,

  INTLIT, INT,

  END
};

struct token
{
  token_type type;
  int intval;
};

const char *tok2string(token_type type);

void dump_tokens(std::vector<token> *tokens);

#endif /* TOKEN_T */
