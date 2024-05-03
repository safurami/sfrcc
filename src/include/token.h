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
  int attribute; // atrribute is index in symbol table
public:
  token() { }
  token(const token&);
  void print_info();
  void set_type(token_type_t);
  void set_attribute(int);
};

token* init_token(void);
void free_token(token *);

#endif
