#ifndef H_TOKEN
#define H_TOKEN

typedef enum {
  // single character tokens
  OPEN_PAREN,
  CLOSE_PAREN,
  SEMICOLON,
  OPEN_CURLYB,
  CLOSE_CURLYB,
  ASSIGN,
  COMA,
  SQ_OPEN_B,
  SQ_CLOSE_B,
  PLUS,
  MINUS,
  STAR,
  DIVIDE,
  LESS,
  GREATER,
  DOLLAR,

  // double
  IS_EQUAL, // ==
  LEQ, // less or equal
  GEQ, // greater or equal
  NEQ, // not equal

  // keywords / other
  IDENTIFIER,
  NUMBER,
  CONS, // like '\n' and '\t'
  LITERAL, // like "Hello" and "World"
  INT,
  CHAR,
  UNSIGNED,
  IF,
  ELSE,
  RETURN,
  CONST,
  FOR,
  WHILE,
  SWITCH,
  LONG,
  CASE,
  STRUCT
} token_type_t;

class token
{
  token_type_t token_type;
  int attribute; // index of lexeme in symbol table
public:
  token() = default;
  ~token() = default;
  token(token&) = delete;
  void set_attribute(int);
  int get_attribute(int);
  token_type_t get_type();
  void set_type(token_type_t);
};

#endif
