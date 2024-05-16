/*
 * Functions for working with lexer.
 */
#include "include/token.h"
#include "include/lexer.h"
#include "include/my.h"

#include <iostream>

lexer::lexer(char *f)
{
  this->file.open(f);
  if(!file.is_open())
  {
    this->buffer1 = nullptr; // Error, nothing is allocating
    this->buffer2 = nullptr;
    this->current_token = nullptr;
    return;
  }
  this->current_token = new token();
  this->buffer1 = new char[4097](); // 4096 + 1 for null byte
  this->buffer2 = new char[4097]();
  this->file.read(this->buffer1, 4096);
  this->lexeme_begin = buffer1;
  this->forward = buffer1;
  this->current_line = 1;
}

bool lexer::is_success()
{
  if(this->buffer1 == nullptr)
    return false;
  return true;
}

lexer::~lexer()
{
  if(this->file.is_open())
  {
    this->file.close();
  }
  delete this->current_token;
  delete[] this->buffer1;
  delete[] this->buffer2;
}

int lexer::get_current_line()
{
  return this->current_line;
}

void lexer::fail()
{
  printf("Lexical error on line %d\n", this->current_line);
  exit(1);
}

int lexer::get_next_state(int state, char sym) // core of the DFA
{                                              // TODO maybe make transition table in the future
  switch(state)
  {
    case 0: // initial state
      switch(sym)
      {
        case '(': return 1; break; // open paren F
        case ')': return 2; break; // close paren F
        case ';': return 3; break; // semicolon F
        case '{': return 4; break; // open curly brace F
        case '}': return 5; break; // close curly brace F
        case '=': return 6; break; // assign NOT F
        case ',': return 7; break; // coma F
        case '[': return 8; break; // open square brace F
        case ']': return 9; break; // close square brace F
        case '+': return 10; break; // plus F
        case '-': return 11; break; // minus F
        case '*': return 12; break; // star F
        case '/': return 13; break; // divide F
        case '<': return 14; break; // less NOT F
        case '>': return 15; break; // grater NOT F
        case '.': return 16; break; // dot F
        case '!': return 17; break; // start of "!=" token NOT F
        case '\'': return 18; break; // start of CONS token NOT F
        case '"': return 19; break; // start of LITTERAL token NOT F
        case 'i': return 20; break; // start of INT or IF token NOT F
        case 'c': return 21; break; // start of CHAR or CONST or CASE token NOT F
        case 'v': return 22; break; // start of VOID token NOT F
        case 'u': return 23; break; // start of UNSIGNED token NOT F
        case 'f': return 24; break; // start of FLOAT or FOR token NOT F
        case 'd': return 25; break; // start of DOUBLE token NOT F
        case 's': return 26; break; // start of SIGNED or SHORT or SWITCH or STRUCT token NOT F
        case 'e': return 27; break; // start of ELSE token NOT F
        case 'r': return 28; break; // start of RETURN token NOT F
        case 'w': return 29; break; // start of WHILE token NOT F
        case 'b': return 30; break; // start of BREAK token NOT F
        case 'l': return 31; break; // start of LONG token NOT F
        default:
          if(my::isalpha(sym)) { return 32; } // 32 - STATE FOR IDENTIFIER
          else if(my::isdigit(sym)) { return 33; } // 33 - STATE FOR NUMBERS
          else this->fail();
          break;
      }
      break;
    case 6: // =.
      switch(sym)
      {
        case '=': return 34; break; // ==. F, == token
        default: return 35; break; // =. F, = token
      }
      break;
    case 14: // <.
      switch(sym)
      {
        case '=': return 36; break; // <=. F, <= token
        default: return 37; break; // <. F, < token
      }
      break;
    case 15: // >.
      switch(sym)
      {
        case '=': return 38; break; // >=. F, >= token
        default: return 39; break; // >. F, > token
      }
      break;
    case 17: // !.
      switch(sym)
      {
        case '=': return 40; break; // !=. F, != token
        default: this->fail();
      }
      break;
    case 18: //'.
      switch(sym)
      {
        case '\'': this->fail(); // fail, '' is not allowed
        default: return 41; break; // '. NOT F
      }
      break;
    case 19: //".
      switch(sym)
      {
        case '"': this->fail();
        //case '"': return 43; break; " ... ". F, LITERAL token
        default: return 65; break; // ",. " NOT F
      }
      break;
    case 20: // i.
      switch(sym)
      {
        case 'f': return 44; break; // if. NOT F, IF token
        case 'n': return 45; break; // in.t NOT F, INT token
        default: return 32; break; // return state for identifiers
      }
      break;
    case 21: // c. - c.har/c.onst/c.ase
      switch(sym)
      {
        case 'h': return 46; break; // ch.ar NOT F, CHAR token
        case 'o': return 47; break; // co.nst NOT F, CONST token
        case 'a': return 48; break; // ca.se NOT F, CASE TOKEN
        default: return 32; break; // identifier
      }
      break;
    case 22: // v.oid
      switch(sym)
      {
        case 'o': return 49; break; // vo.id NOT F, VOID token
        default: return 32; break; // identifier
      }
      break;
    case 23: // u.nsigned
      switch(sym)
      {
        case 'n': return 50; break; // un.signed NOT F, UNSIGNED token
        default: return 32; break;
      }
      break;
    case 24: // f.loat / f.or
      switch(sym)
      {
        case 'l': return 51; break; // fl.oat NOT F
        case 'o': return 52; break; // fo.r NOT F
        default: return 32; break;
      }
      break;
    case 25: //d.ouble
      switch(sym)
      {
        case 'o': return 53; break; // do.uble NOT F
        default: return 32; break;
      }
      break;
    case 26: // s.igned / s.hort / s.truct / s.witch
      switch(sym)
      {
        case 'i': return 54; break; // si.gned NOT F
        case 'h': return 55; break; // sh.ort NOT F
        case 't': return 56; break; // st.ruct NOT F
        case 'w': return 57; break; // sw.itch NOT F
        default: return 32; break;
      }
      break;
    case 27: // e.lse
      switch(sym)
      {
        case 'l': return 58; break; // el.se NOT F
        default: return 32; break;
      }
      break;
    case 28: // r.eturn
      switch(sym)
      {
        case 'e': return 59; break; // re.turn NOT F
        default: return 32; break;
      }
      break;
    case 29: // w.hile
      switch(sym)
      {
        case 'h': return 60; break; // wh.ile NOT F
        default: return 32; break;
      }
      break;
    case 30: // b.reak
      switch(sym)
      {
        case 'r': return 61; break; // br.ake NOT F
        default: return 32; break;
      }
      break;
    case 31: // l.ong
      switch(sym)
      {
        case 'o': return 62; break; // lo.ng NOT F
        default: return 32; break;
      }
      break;
    case 32: // for identifiers
      if(!my::isalnum(sym)) { return 63; } // 63 - state for finished identifiers
      else { return 32; }
      break;
    case 33: // for numbers
      if(!my::isdigit(sym)) { return 64; } // 64 - state for finished numbers
      else { return 33; }
      break;
    case 41: // ',. '
      switch(sym)
      {
        case '\'': return 42; break; // ','. F
        default: this->fail();
      }
      break;
    case 44: // if.
      if(!my::isalnum(sym)) { return 66; } // if. F, IF token
      else { return 32; } // identifier
    case 45: // in.t
      switch(sym)
      {
        case 't': return 67; break; // int. NOT F YET
        default: return 32; break;
      }
      break;
    case 46: // ch.ar
      switch(sym)
      {
        case 'a': return 68; break; // cha.r NOT F
        default: return 32; break;
      }
      break;
    case 47: // co.nst
      switch(sym)
      {
        case 'n': return 69; break; // con.st NOT F
        default: return 32; break;
      }
      break;
    case 48: // ca.se
      switch(sym)
      {
        case 's': return 70; break; // cas.e NOT F
        default: return 32; break;
      }
      break;
    case 49: // vo.id
      switch(sym)
      {
        case 'i': return 71; break; // voi.d NOT F
        default: return 32; break;
      }
      break;
    case 50: //un.signed
      switch(sym)
      {
        case 's': return 72; break; // uns.igned NOT F
        default: return 32; break;
      }
      break;
    case 51: // fl.oat
      switch(sym)
      {
        case 'o': return 73; break; // flo.at NOT F
        default: return 32; break;
      }
      break;
    case 52: // fo.r
      switch(sym)
      {
        case 'r': return 74; break; // for. NOT F
        default: return 32; break;
      }
      break;
    case 53: // do.uble
      switch(sym)
      {
        case 'u': return 75; break; // dou.ble NOT F
        default: return 32; break;
      }
      break;
    case 54: // si.gned
      switch(sym)
      {
        case 'g': return 76; break; // sig.ned NOT F
        default: return 32; break;
      }
      break;
    case 55: // sh.ort
      switch(sym)
      {
        case 'o': return 77; break; // sho.rt NOT F
        default: return 32; break;
      }
      break;
    case 56: // st.ruct
      switch(sym)
      {
        case 'r': return 78; break; // str.uct NOT F
        default: return 32; break;
      }
      break;
    case 57: // sw.itch
      switch(sym)
      {
        case 'i': return 79; break; // swi.tch NOT F
        default: return 32; break;
      }
      break;
    case 58: // el.se
      switch(sym)
      {
        case 's': return 80; break; // els.e NOT F
        default: return 32; break;
      }
      break;
    case 59: // re.turn
      switch(sym)
      {
        case 't': return 81; break;  //ret.urn NOT F
        default: return 32; break;
      }
      break;
    case 60: // wh.ile
      switch(sym)
      {
        case 'i': return 82; break; // whi.le NOT F
        default: return 32; break;
      }
      break;
    case 61: // br.eak
      switch(sym)
      {
        case 'e': return 83; break; // bre.ak NOT F
        default: return 32; break;
      }
      break;
    case 62: // lo.ng
      switch(sym)
      {
        case 'n': return 84; break; // lon.g NOT F
        default: return 32; break;
      }
      break;
    case 65: // ",. "
      if(sym == '"') { return 43; } // 43 finished state for literal
      else { return 65; }
      break;
    case 67: // int.
      if(!my::isalnum(sym)) { return 85; } // int. F
      else { return 32; }
      break;
    case 68: // cha.r
      switch(sym)
      {
        case 'r': return 86; break; // char. NOT F
        default: return 32; break;
      }
      break;
    case 69: // con.st
      switch(sym)
      {
        case 's': return 87; break; // cons.t NOT F
        default: return 32; break;
      }
      break;
    case 70: // cas.e
      switch(sym)
      {
        case 'e': return 88; break; // case. NOT F
        default: return 32; break;
      }
      break;
    case 71: // voi.d
      switch(sym)
      {
        case 'd': return 89; break; // void. NOT F
        default: return 32; break;
      }
      break;
    case 72: // uns.igned
      switch(sym)
      {
        case 'i': return 90; break; // unsi.gned NOT F
        default: return 32; break;
      }
      break;
    case 73: // flo.at
      switch(sym)
      {
        case 'a': return 91; break; // floa.t NOT F
        default: return 32; break;
      }
      break;
    case 74: // for.
      if(!my::isalnum(sym)) { return 92; } // for. F
      else { return 32; }
      break;
    case 75: // dou.ble
      switch(sym)
      {
        case 'b': return 93; break; // doub.le NOT F
        default: return 32; break;
      }
      break;
    case 76: //sig.ned
      switch(sym)
      {
        case 'n': return 94; break; // sign.ed NOT F
        default: return 32; break;
      }
      break;
    case 77: // sho.rt
      switch(sym)
      {
        case 'r': return 95; break; //shor.t NOT F
        default: return 32; break;
      }
      break;
    case 78: // str.uct
      switch(sym)
      {
        case 'u': return 96; break; // stru.ct NOT F
        default: return 32; break;
      }
      break;
    case 79: // swi.tch
      switch(sym)
      {
        case 't': return 97; break; // swit.ch NOT F
        default: return 32; break;
      }
      break;
    case 80: // els.e
      switch(sym)
      {
        case 'e': return 98; break; // else. NOT F
        default: return 32; break;
      }
      break;
    case 81: // ret.urn
      switch(sym)
      {
        case 'u': return 99; break; //retu.rn NOT F
        default: return 32; break;
      }
      break;
    case 82: // whi.le
      switch(sym)
      {
        case 'l': return 100; break; // whil.e NOT F
        default: return 32; break;
      }
      break;
    case 83: // bre.ak
      switch(sym)
      {
        case 'a': return 101; break; // brea.k NOT F
        default: return 32; break;
      }
      break;
    case 84: // lon.g
      switch(sym)
      {
        case 'g': return 102; break; // long. NOT F
        default: return 32; break;
      }
      break;
    case 86: // char.
      if(!my::isalnum(sym)) { return 103; } // char. F
      else { return 32; }
      break;
    case 87: // cons.t
      switch(sym)
      {
        case 't': return 104; break; // const. NOT F
        default: return 32; break;
      }
      break;
    case 88: // case.
      if(!my::isalnum(sym)) { return 105; } // case. F
      else { return 32; }
      break;
    case 89: // void.
      if(!my::isalnum(sym)) { return 106; } // void. F
      else { return 32; }
      break;
    case 90: // unsi.gned
      switch(sym)
      {
        case 'g': return 107; break; // unsig.ned NOT F
        default: return 32; break;
      }
      break;
    case 91: //floa.t
      switch(sym)
      {
        case 't': return 108; break; // float. NOT F
        default: return 32; break;
      }
      break;
    case 93: //doub.le
      switch(sym)
      {
        case 'l': return 109; break; // doubl.e NOT F
        default: return 32; break;
      }
      break;
    case 94: //sign.ed
      switch(sym)
      {
        case 'e': return 110; break; // signe.d NOT F
        default: return 32; break;
      }
      break;
    case 95: //shor.t
      switch(sym)
      {
        case 't': return 111; break; // short. NOT F
        default: return 32; break;
      }
      break;
    case 96: // stru.ct
      switch(sym)
      {
        case 'c': return 112; break; //struc.t NOT F
        default: return 32; break;
      }
      break;
    case 97: // swit.ch
      switch(sym)
      {
        case 'c': return 113; break; // switc.h NOT F
        default: return 32; break;
      }
      break;
    case 98: // else.
      if(!my::isalnum(sym)) { return 114; } // else. F
      else { return 32; }
      break;
    case 99: // retu.rn
      switch(sym)
      {
        case 'r': return 115; break; // retur.n NOT F
        default: return 32; break;
      }
      break;
    case 100: // whil.e
      switch(sym)
      {
        case 'e': return 116; break; // while. NOT F
        default: return 32; break;
      }
      break;
    case 101: // brea.k
      switch(sym)
      {
        case 'k': return 117; break; // break. NOT F
        default: return 32; break;
      }
      break;
    case 102: // long.
      if(!my::isalnum(sym)) { return 118; } // long. F
      else { return 32; }
      break;
    case 104: // const.
      if(!my::isalnum(sym)) { return 119; } // const. F
      else { return 32; }
      break;
    case 107: // unsig.ned
      switch(sym)
      {
        case 'n': return 120; break; // unsign.ed NOT F
        default: return 32; break;
      }
      break;
    case 108: // float
      if(!my::isalnum(sym)) { return 121; } // float. F
      else { return 32; }
      break;
    case 109: // doubl.e
      switch(sym)
      {
        case 'e': return 122; break; // double. NOT F
        default: return 32; break;
      }
      break;
    case 110: // signe.d
      switch(sym)
      {
        case 'd': return 123; break; // signed. NOT F
        default: return 32; break;
      }
      break;
    case 111: // short.
      if(!my::isalnum(sym)) { return 124; } // short. F
      else { return 32; }
      break;
    case 112: // struc.t
      switch(sym)
      {
        case 't': return 125; break; // struct. NOT F
        default: return 32; break;
      }
      break;
    case 113: //switc.h
      switch(sym)
      {
        case 'h': return 126; break; // switch. NOT F
        default: return 32; break;
      }
      break;
    case 115: // retur.n
      switch(sym)
      {
        case 'n': return 127; break; // return. NOT F
        default: return 32; break;
      }
      break;
    case 116: // while.
      if(!my::isalnum(sym)) { return 128; } // while. F
      else { return 32; }
      break;
    case 117: // break.
      if(!my::isalnum(sym)) { return 129; } // break. F
      else { return 32; }
      break;
    case 120: // unsign.ed
      switch(sym)
      {
        case 'e': return 130; break; // unsigne.d NOT F
        default: return 32; break;
      }
      break;
    case 122: // double.
      if(!my::isalnum(sym)) { return 131; } // double. F
      else { return 32; }
      break;
    case 123: // signed.
      if(!my::isalnum(sym)) { return 132; } // signed. F
      else { return 32; }
      break;
    case 125: // struct
      if(!my::isalnum(sym)) { return 133; } // struct. F
      else { return 32; }
      break;
    case 126: // switch.
      if(!my::isalnum(sym)) { return 134; } // switch. F
      else { return 32; }
      break;
    case 127: // return.
      if(!my::isalnum(sym)) { return 135; } // return. F
      else { return 32; }
      break;
    case 130: // unsigne.d
      switch(sym)
      {
        case 'd': return 136; break; // unsigned. NOT F
        default: return 32; break;
      }
      break;
    case 136: // unsigned.
      if(!my::isalnum(sym)) { return 137; } // unsigned. F
      else { return 32; }
      break;
    default:
      printf("How did you get here\n");
      break;
  }
  return 0;
}

int lexer::is_success_state(int state) {} // TODO

token* lexer::get_next_token()
{
  int state = 0; // initial state of DFA
  char symbol = *forward++;
  while(1)
  {

  switch(symbol)
  {
    case '\0':
      if(this->forward == this->buffer1 + 4096)
      {
        this->file.read(this->buffer2, 4096);
        this->forward = this->buffer2;
      }
      else if(this->forward == this->buffer2 + 4096)
      {
        this->file.read(this->buffer1, 4096);
        this->forward = this->buffer1;
      }
      else
      {
        this->current_token->set_type(token_type::DOLLAR);
        return this->current_token;
      }
      break;
    case '\n':
      this->current_line++;
      break;
  }

  }

  return this->current_token;
}
