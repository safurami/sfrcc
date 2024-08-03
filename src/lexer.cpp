#include <cassert>

#include "include/lexer.h"
#include "include/my.h"


Lexer::Lexer(std::ifstream *file): line(1), putback(0), file(file), was_error(false) {}
// Get next character from input file, including '\n', eof and etc.
// If character is '\n', increase this->m_file.
char Lexer::next(void)
{
  char c;

  // If there is some character in putback, return it.
  // Otherwise read from file.
  if(this->putback)
  {
    c = this->putback;
    this->putback = 0;
    return c;
  }

  c = this->file->get();
  if(c == '\n')
  {
    this->line++;
  }
  return c;
}

void Lexer::putback_char(const char c)
{
  // Assertion to ensure that there is not cases whene
  // during calling putback_char() there is already character
  // in this->putback.
  if(this->putback)
  {
    assert(false);
  }
  this->putback = c;
}

// Return next character from input, skip '\n', '\t', and etc.
char Lexer::skip(void)
{
  char c;

  // Loop for skipping not suitable characters.
  while((c = this->next()), c == '\t' || c == '\n' || c == ' ') {}
  return c;
}

// Scans sequence of integers in input stream.
// Return integer that was found.
// @val is the first parsed number in the sequence.
int Lexer::scanint(char val)
{
  int sum = val - '0';
  char c;
  while((c = this->next()), my::isdigit(c))
  {
    sum = (sum * 10) + (c - '0');
  }
  this->putback_char(c);
  return sum;
}

// Get next token from the input.
// Return 1 if found token, 0 if End Of File, -1 on error.
// If found token, found token is placed to @tok.
int Lexer::get_tok(token *tok)
{
  char c;
  c = this->skip();
  switch(c)
  {
    case EOF:
      return 0;
    case '+':
      tok->type = token_type::PLUS;
      break;
    case '-':
      tok->type = token_type::MINUS;
      break;
    case '*':
      tok->type = token_type::STAR;
      break;
    case '/':
      tok->type = token_type::SLASH;
      break;
    case ';':
      tok->type = token_type::SEMICOLON;
      break;
    default:
      if(my::isdigit(c))
      {
        tok->intval = this->scanint(c);
        tok->type = token_type::INTLIT;
        break;
      }

      printf("Unexpected character '%c' on line %d\n", c, this->line);
      //exit(1); // TODO: make error handling instead.
      return -1;
  }

  // Found a token, returning 1(true).
  return 1;
}

// Scanning the whole file and on success, return pointer to vector
// with tokens. Otherwise return nullptr, and was_error set to true.
std::vector<token>* scan(Lexer *lexer)
{
  token tok;
  std::vector<token> *vec = new std::vector<token>();

  vec->reserve(20);
  int ret;

  // TODO: add something to handle errors.
  while((ret = lexer->get_tok(&tok)), ret != 0)
  {
    if(ret == -1)
    {
      lexer->was_error = true;
      continue;
    }
    
    // If there was an error, then it makes no sence to push new tokens
    // to vector, because vector will be freed at the end of scanning.
    // I think this will improve perfomance.
    if(!lexer->was_error)
    {
      vec->push_back(tok);
    }
  }

  if(lexer->was_error)
  {
    delete vec;
    return nullptr;
  }

  // Adding EOF at the end of tokens.
  vec->push_back({token_type::END, 0});
  return vec;
}

// Just put it here, because i dont know where else.
const char *tok2string(token_type type)
{
  int size = 7;
  const char *tokens[] = {"Plus", "Minus", "Star", "Slash", "Int Lit", "Semicolon", "End"};
  assert((int)type < size); // TODO: remove when all tokens are defined.
  return tokens[(int)type];
}
