/*
 * My implementation of standard functions
 */

#include "include/my.h"
#include <cstdio>
#include <cstdlib>


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

my::timer::timer()
{
  this->m_start_point = std::chrono::high_resolution_clock::now();
};


my::timer::~timer()
{
  auto end_point = std::chrono::high_resolution_clock::now();

  auto start = std::chrono::time_point_cast<std::chrono::microseconds>(this->m_start_point).time_since_epoch().count();
  auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();

  long duration = end - start;

  printf("Operation took %ldus(%0.2fms)\n",duration, static_cast<double>(duration)/1000 );
}

/*
 * My implementation of standrard assert macro and function
 */
void my::assert(const bool expr, const char *file, const int line, const char *func)
{
  if(!expr)
  {
    printf("%s:%d : Assert failed in function %s\n", file, line, func);
    std::abort();
  }
}
