#ifndef H_TOKEN
#define H_TOKEN

typedef enum {
  // single character tokens
  OPEN_PAREN,
  CLOSE_PAREN,
  SEMICOLON,
  OPEN_CURLYB,
  CLOSE_CURLYB,
  EQUAL,
  COMA,
  SQ_OPEN_B,
  SQ_CLOSE_B,
  QUOTE,
  PLUS,
  MINUS,
  STAR,
  DIVIDE,
  DOLLAR,

  // double
  DOUBLE_EQUAL,

  // keywords / other
  IDENTIFIER,
  NUMBER,
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
  char *lexeme;
public:
  token();
  ~token();
  token(token&) = delete;
  void set_lexeme(const char *, const char *);
  token_type_t get_type();
  void set_type(token_type_t);
  char *get_lexeme(); // REMOVEME just for debug
};

#endif
