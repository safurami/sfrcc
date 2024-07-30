#ifndef TOKEN_H
#define TOKEN_H

enum class token_type {
  PLUS, MINUS, STAR, SLASH,

  INTLIT,

  END
};

struct token
{
  token_type type;
  int intval;
};

const char *tok2string(token_type type);


#endif /* TOKEN_T */
