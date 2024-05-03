#ifndef H_LEXER
#define H_LEXER

#include "./token.h"
#include <iostream>
#include <vector>

void start_lexer(token *,std::ifstream&);

class lexer
{
  int current_line;
  char *string;
  token *tok;
  std::vector<token> vec;
public:
  lexer(lexer &) = delete;
  lexer(token *);
  // ~lexer();
  std::vector<token> get_tokens(std::ifstream&);
  char get_char();
  void unchar(int);
  void push_tok_to_stacK(token_type_t, int);
};

// single token
void check_openp(lexer*);
void check_close_p(lexer*);
void check_semicolon(lexer *);
void check_opencb(lexer *);
void check_closecb(lexer *);
void check_equal(lexer *);
void check_coma(lexer *);
void check_sqopenb(lexer *);
void check_sqcloseb(lexer *);
void check_quote(lexer *);
void check_start(lexer *);
void check_plus(lexer *);
void check_minus(lexer *);
void check_times(lexer *);
void check_divide(lexer *);

// double
void check_double_eq(lexer *);

// keywords / etc
void check_identifier(lexer *);
void check_number(lexer *);
void check_int(lexer *);
void check_char(lexer *);
void check_unsigned(lexer *);
void check_if(lexer *);
void check_else(lexer *);
void check_return(lexer *);
void check_const(lexer *);
void check_for(lexer *);
void check_while(lexer *);
void check_switch(lexer *);
void check_long(lexer *);
void check_case(lexer *);
void check_struct(lexer *);

#endif
