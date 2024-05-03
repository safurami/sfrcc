/*
 * My inplementation of standart functions
 */

#include "include/my.h"

namespace my
{

bool isdigit(char sym)
{
  return sym >= '0' && sym <= '9';
}

bool isalnum(char sym)
{
  return (sym >= '0' && sym <= '9') || (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z');
}

int strlen(const char *string)
{
  int i;
  for(i = 0; *string != '\0'; i++, string++) {}
  return i;
}

};
