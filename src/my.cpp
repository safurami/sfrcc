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
 * Reads whole string from file, then copy it
 * to storage, copied string terminates with null byte.
 * Returns number of reads characters, or EOF if there
 * are no more strings.
 */
int fgets(char *storage, std::ifstream& file)
{
  char c = 0;
  int i = 0;
  for(;;i++)
  {
    file.read(&c, 1);
    if(c != '\n' && c != 0)
    {
      *(storage + i) = c;
    }
    else
    {
      *(storage + i) = '\0';
      break;
    }
  }
  if(i == 0)
    return EOF;
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
