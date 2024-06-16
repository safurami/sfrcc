#include "include/compiler.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Usage: %s <file(s).c>\n", argv[0]);
    return EXIT_FAILURE;
  }

  compiler comp(argv[1]);

  return EXIT_SUCCESS;
}
