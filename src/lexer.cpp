/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

lexer::lexer(const char *f, symbol_table* table)
{
  this->m_buffer1 = nullptr;
  if(table == nullptr)
  {
    return;
  }
  this->m_table = table;

  this->m_file.open(f);
  if(!m_file.is_open())
  {
    return;
  }

  this->m_was_error = false;
  this->m_current_token = new token();
  this->m_buffer1 = new char[4097](); // 4096 + 1 for null byte.
  this->m_buffer2 = new char[4097]();
  this->m_file.read(this->m_buffer1, 4096);
  this->m_lexeme_begin = m_buffer1;
  this->m_forward = m_buffer1;
  this->m_current_line = 1;
}

void lexer::free_all()
{
  delete[] this->m_buffer1;
  delete[] this->m_buffer2;
  delete this->m_current_token;
  if(this->m_file.is_open())
  {
    this->m_file.close();
  }
  this->m_forward = nullptr;
  this->m_lexeme_begin = nullptr;
}

char lexer::peek()
{
  int i;
  for(i = 0; this->m_forward[i] == '\n'; i++) {}
  return this->m_forward[i];
}

bool lexer::is_success()
{
  return this->m_buffer1 != nullptr;
}

lexer::~lexer()
{
  this->free_all();
}

int lexer::get_current_line()
{
  return this->m_current_line;
}

bool lexer::was_error()
{
  return this->m_was_error;
}

void lexer::lexer_error() // TODO: possible memory leak i guess, and prefered to refactor this function.
{
#ifdef DEBUG
  printf("[Lexer] Detected an error...\n");
#endif
  this->m_was_error = true;
  char *error_place = this->m_forward - 1; // decrementing because after next_character, forward pointer increments by one.
  char *move_pointer = this->m_lexeme_begin; // pointer for moving and printing characters

  printf("\n[line %d]: Unexpected character '%c'.\n", this->m_current_line ,*error_place);

  char *string_start;

  if(this->m_current_line == 1)
  {
    string_start = this->m_buffer1;
  }
  else
  {
    for(;*(move_pointer-1) != '\n'; move_pointer--) {}
    string_start = move_pointer;
  }
  for(move_pointer = string_start; *move_pointer != '\n'; move_pointer++) { printf("%c", *move_pointer); }
  printf("\n");
  for(move_pointer = string_start; move_pointer != error_place; move_pointer++)
  {
    printf("-");
  }
  printf("^\n");

  // terminating by first error.
  /*
  this->free_all();
  exit(1);
  */
}

bool lexer::match(char sym)
{
  if(this->next_character() != sym)
  {
    this->m_forward--;
    return false;
  }
  return true;
}

bool lexer::check_word(const char *word)
{
  for(int i = 0; word[i] != '\0'; i++)
  {
    if(!this->match(word[i]))
    {
       return false;
    }
  }
  return true;
}

void lexer::collect_number()
{
  this->m_forward = this->m_lexeme_begin;
  for(;my::isdigit(this->peek()); this->next_character()) {}
  this->m_current_token->set_type(token_type::NUMBER, this->m_current_line);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin, this->m_forward - 1, node_type::NUMBER)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_id()
{
  this->m_forward = this->m_lexeme_begin;
  for(; my::isalnum(this->peek()); this->next_character()) {}
  this->m_current_token->set_type(token_type::IDENTIFIER, this->m_current_line);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin, this->m_forward - 1, node_type::IDENTIFIER)); // TODO: fix loop, loop reads one more character than needed.
}

void lexer::collect_literal()
{
  int c;
  this->reset_lexeme();
  // TODO: add solution to avoid in case "some literal... END OF FIRST BUFFER  some literal...".
  // Maybe try to copy character by character to special buffer.
  while((c = this->next_character()), c != '"')
  {
    if(c == EOF) // User missed terminating ", so switch statement in get_next_token() will catch this situation.
    {
      printf("Error: Unterminated string.\n");
      printf("TODO: Add error handling for this\n");
      return;
    }
  }
  this->m_current_token->set_type(token_type::LITERAL, this->m_current_line);
  this->m_current_token->set_attribute(this->m_table->install_node(this->m_lexeme_begin + 1, this->m_forward - 2, node_type::LITERAL)); // TODO: fix loop, loop reads two mode character than needed.
}

/*
 * Function for getting current character and increasing
 * m_forward pointer, if m_forward pointer now points to null byte -
 * check, if it is an end of either buffer, then read m_file to another buffer
 * and switch m_forward pointer there, otherwise return EOF. If it is not null byte -
 * return character that was read.
 */
char lexer::next_character()
{
  char *tmp;
  for(; (tmp = this->m_forward), *tmp == '\0' || *tmp == '\n';)
  {
    if(*tmp == '\0')
    {
      if(tmp == this->m_buffer1 + 4096) // End of first buffer
      {
#ifdef DEBUG
        printf("[Lexer]: Occured end of first buffer\n");
#endif

        for(int i = 0; i < 4097; i++) // Clearing the buffer.
        {
          this->m_buffer2[i] = '\0';
        }

        this->m_file.read(this->m_buffer2, 4096);
        this->m_forward = this->m_buffer2;
        this->m_lexeme_begin = this->m_forward;
#ifdef DEBUG
        printf("[Lexer] Buffer2 start on %p\n", this->m_buffer2);
        printf("[Lexer] After switching buffers, forward pointer points to %p(\'%c\')\n",this->m_forward, *this->m_forward);
#endif
      }
      else if(tmp == this->m_buffer2 + 4096) // End of second buffer
      {
#ifdef DEBUG
        printf("[Lexer] occured end of second buffer\n");
#endif

        for(int i = 0; i < 4097; i++) // Clearing the buffer.
        {
          this->m_buffer1[i] = '\0';
        }

        this->m_file.read(this->m_buffer1, 4096);
        this->m_forward = this->m_buffer1;
        this->m_lexeme_begin = this->m_forward;
#ifdef DEBUG
        printf("[Lexer] Buffer1 start on %p\n", this->m_buffer1);
        printf("[Lexer] After switching buffers, forward pointer points to %p(\'%c\')\n",this->m_forward, *this->m_forward);
#endif
      }
      else
      {
#ifdef DEBUG
        printf("[Lexer] Detected EOF, returning EOF...\n");
#endif
        return EOF;
      }
    }
    if(*tmp == '\n')
    {
#ifdef DEBUG
      printf("[Lexer] Occured new line\n");
#endif
      this->m_current_line++;
      this->m_forward++;
      //this->m_lexeme_begin++; // without this increment, lexeme_begin will point to '\n', and check_word() will alway fail.
      this->m_lexeme_begin = this->m_forward;
    }
  }
  this->m_forward++;
  return *tmp;
}

void lexer::skip_line_comment()
{
  // TODO: implemented via raw pointer. That will spoil all when buffer1 will end. Implement via next_character().
  for(;*this->m_forward++ != '\n';) {}
  this->m_current_line++; // New line occurred
  this->m_lexeme_begin = this->m_forward;
}

void lexer::skip_multline_comment()
{
  while(1)
  {
    if(this->match('*') && this->match('/'))
    {
      break;
    }
    this->next_character();
  }
  this->m_lexeme_begin = this->m_forward;
}

/*
 * Function to reset lexeme.
 * This function assign this->m_forward to this->m_lexeme_begin + 1.
 * +1 because first symbol is already read by switch statement in
 * get_next_token().
 * Returns true just for comfortable using in if statements.
 */
bool lexer::reset_lexeme()
{
  this->m_forward = this->m_lexeme_begin + 1;
  return true;
}

token* lexer::get_next_token()
{
  char symbol;
again: // label to avoid returning token after whitespace or comment
  switch(symbol = this->next_character())
  {
    case EOF:
      this->m_current_token->set_type(token_type::DOLLAR, this->m_current_line);
#ifdef DEBUG
      printf("[Lexer] EOF IN CASE STATEMENT\n");
      printf("[Lexer] Returning DOLLAR token...\n");
#endif
      break;
    case ' ': this->m_lexeme_begin = this->m_forward; goto again;
    case '\t': this->m_lexeme_begin = this->m_forward; goto again;
    case '(':
      this->m_current_token->set_type(token_type::OPEN_PAREN, this->m_current_line);
      break;
    case ')':
      this->m_current_token->set_type(token_type::CLOSE_PAREN, this->m_current_line);
      break;
    case ';':
      this->m_current_token->set_type(token_type::SEMICOLON, this->m_current_line);
      break;
    case '{':
      this->m_current_token->set_type(token_type::OPEN_CURLYB, this->m_current_line);
      break;
    case '}':
      this->m_current_token->set_type(token_type::CLOSE_CURLYB, this->m_current_line);
      break;
    case '=':
      this->m_current_token->set_type(this->match('=') ? token_type::IS_EQUAL : token_type::ASSIGN, this->m_current_line);
      break;
    case ',':
      this->m_current_token->set_type(token_type::COMMA, this->m_current_line);
      break;
    case '[':
      this->m_current_token->set_type(token_type::SQ_OPEN_B, this->m_current_line);
      break;
    case ']':
      this->m_current_token->set_type(token_type::SQ_CLOSE_B, this->m_current_line);
      break;
    case '+':
      this->m_current_token->set_type(token_type::PLUS, this->m_current_line);
      break;
    case '-':
      this->m_current_token->set_type(token_type::MINUS, this->m_current_line);
      break;
    case '*':
      this->m_current_token->set_type(token_type::STAR, this->m_current_line);
      break;
    case '/':
      if(this->match('/'))
      {
        this->skip_line_comment();
        goto again;
      }
      else if(this->match('*'))
      {
        this->skip_multline_comment();
        goto again;
      }
      else
      {
        this->m_current_token->set_type(token_type::DIVIDE, this->m_current_line);
      }
      break;
    case '<':
      this->m_current_token->set_type( this->match('=') ? token_type::LEQ : token_type::LESS, this->m_current_line);
      break;
    case '>':
      this->m_current_token->set_type( this->match('=') ? token_type::GEQ : token_type::GREATER, this->m_current_line);
      break;
    case '.':
      this->m_current_token->set_type(token_type::DOT, this->m_current_line);
      break;
    case '!':
      this->m_current_token->set_type(this->match('=') ? token_type::NEQ : token_type::BANG, this->m_current_line);
      break;
    case 'i':
      if(this->match('f') && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::IF, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("nt") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::INT, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case '\'':
      this->m_current_token->set_attribute(this->next_character());
      if(!match('\''))
      {
        this->m_forward++; // just because in lexer_error() forward decrementing by one.
        this->lexer_error();

        // for case if we dont terminate lexer after first error.
        for(; this->next_character() != '\'';) {} // Occured error, more than one character in quotes, skiping until closing quote.
        this->m_lexeme_begin = this->m_forward;
        goto again;
      }
      this->m_current_token->set_type(token_type::CONS, this->m_current_line);
      break;
    case '"':
      this->collect_literal();
      break;
    case 'c':
      if(this->check_word("har") &&
         !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::CHAR, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("onst") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::CONST, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("ase") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::CASE, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'v':
      if(this->check_word("oid") &&
         !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::VOID, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'u':
      if(this->check_word("nsigned") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::UNSIGNED, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'f':
      if(this->check_word("loat") &&
         !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::FLOAT, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("or") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::FOR, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'd':
      if(this->check_word("ouble") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::DOUBLE, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 's':
      if(this->check_word("igned") &&
         !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::SIGNED, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("hort") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::SHORT, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("witch") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::SWITCH, this->m_current_line);
      }
      else if(this->reset_lexeme() &&
              this->check_word("truct") &&
              !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::STRUCT, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'e':
      if(this->check_word("lse") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::ELSE, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'r':
      if(this->check_word("eturn") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::RETURN, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'w':
      if(this->check_word("hile") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::WHILE, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'b':
      if(this->check_word("reak") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::BREAK, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case 'l':
      if(this->check_word("ong") && !my::isalnum(this->peek()))
      {
        this->m_current_token->set_type(token_type::LONG, this->m_current_line);
      }
      else
      {
        this->collect_id();
      }
      break;
    case '&':
      this->m_current_token->set_type(token_type::AND, this->m_current_line);
      break;
    case '|':
      this->m_current_token->set_type(token_type::OR, this->m_current_line);
      break;
    default:
      if(my::isalpha(symbol))
      {
        this->collect_id();
      }
      else if(my::isdigit(symbol))
      {
        this->collect_number();
      }
      else // Something else, for example $ or %
      {
        this->lexer_error();
      }
      break;
  }

  this->m_lexeme_begin = this->m_forward;

#ifdef DEBUG
  printf("[Lexer] Returning token: <Line: %d, Token: %s, Attribute: %d>\n", this->m_current_line, token_type_to_string(this->m_current_token->get_type()), this->m_current_token->get_attribute());
#endif

  return this->m_current_token;
}
