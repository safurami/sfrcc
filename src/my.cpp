/*
 * My inplementation of standard functions
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


/*
 * Reads string until null byte.
 * Returns number of reads bytes, null byte is not included.
 */
int strlen(const char *string)
{
  int i;
  for(i = 0; *string != '\0'; i++, string++) {}
  return i;
}


/*
 * Copy first n bytes from source to destination,
 * after copied string, writes null byte.
 * Return number of copied bytes, null byte is not included.
 */
int strncpy(char *destination, const char *source, int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    *(destination + i) = *(source + i);
  }
  *(destination + i) = '\0';
  return i;
}

};
