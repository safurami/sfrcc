#ifndef H_TOKEN
#define H_TOKEN

enum class token_type: unsigned char {
  // single character tokens
  OPEN_PAREN = 0,
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
  DOLLAR, // for EOF
  DOT,

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
  VOID,
  UNSIGNED,
  FLOAT,
  DOUBLE,
  SIGNED,
  IF,
  SHORT,
  ELSE,
  RETURN,
  CONST,
  FOR,
  WHILE,
  BREAK,
  SWITCH,
  LONG,
  CASE,
  STRUCT
};

class token
{
  token_type type;
  int attribute; // index of lexeme in symbol table
public:
  token() = default;
  ~token() = default;
  token(token&) = delete;
  void set_attribute(int);
  int get_attribute();
  token_type get_type();
  void set_type(token_type);
};

#endif
