#include "include/my.h"

bool my::isdigit(char c)
{
  return c >= '0' && c <= '9';
}

bool my::isalpha(char c)
{
  return (c >= 'A' && c <= 'Z') ||  (c >= 'a' && c <= 'z');
}

bool my::isalnum(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

int my::strlen(const char *string)
{
  int i;
  for(i = 0; string[i] != '\0'; i++) { }
  return i;
}

bool my::strcmp(const char *src, const char *dst)
{
  for(;*src != '\0' && *dst != '\0'; src++, dst++)
  {
    if(*src != *dst)
    {
      return false;
    }
  }
  if(*src == '\0' && *dst == '\0')
  {
    return true;
  }
  return false;
}

bool my::strcmpn(const char *src, const char *dst, int count)
{
  int i;
  for(i = 0; i < count && (src[i] != '\0' && dst[i] != '\0'); i++)
  {
    if(src[i] != dst[i])
    {
      return false;
    }
  }
  if(i == count)
  {
    return true;
  }
  return false;
}
