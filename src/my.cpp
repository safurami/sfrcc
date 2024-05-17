/*
 * My inplementation of standard functions
 */

#include "include/my.h"


bool my::isdigit(char sym)
{
  return sym >= '0' && sym <= '9';
}

bool my::isalnum(char sym)
{
  return (sym >= '0' && sym <= '9') || (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z') || sym == '_';
}


/*
 * Reads string until null byte.
 * Returns number of reads bytes, null byte is not included.
 */
int my::strlen(const char *string)
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
int my::strncpy(char *destination, const char *source, int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    *(destination + i) = *(source + i);
  }
  *(destination + i) = '\0';
  return i;
}

bool my::isalpha(char sym)
{
  return (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <='Z') || (sym == '_');
}

/*
 * Compares two strings.
 * Returns true if they are equal each other.
 */
int my::strcmp(const char *str1, const char *str2)
{
  int i;
  for(i = 0; str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0'; i++) {}
  if(str1[i] == '\0' && str2[i] == '\0') { return 1; } // possible bug
  return 0;
}
