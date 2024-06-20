#ifndef H_TOKEN
#define H_TOKEN

enum class token_type: unsigned char {
  // single character tokens
  OPEN_PAREN = 0,
  CLOSE_PAREN,
  SEMICOLON,
  COLON,
  OPEN_CURLYB,
  CLOSE_CURLYB,
  ASSIGN,
  COMMA,
  SQ_OPENB,
  SQ_CLOSEB,
  PLUS,
  MINUS,
  STAR,
  DIVIDE,
  LESS,
  GREATER,
  DOLLAR, // for EOF
  DOT,
  AND, // &
  OR, // |
  BANG, // !

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
private:
  token_type m_type;
  int m_attribute; // index of lexeme in symbol table
  int m_line;
public:
  token();
  ~token() = default;
  token(token&);
  void set_attribute(int);
  int get_attribute();
  token_type get_type();
  void set_parameters(token_type, int);
  void operator =(const token&);
  int get_line();
};

const char* token_type_to_string(token_type); // REMOVEME, just for debug


#endif
