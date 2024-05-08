#ifndef H_MY
#define H_MY

#include <fstream>

namespace my
{

bool isdigit(char);
bool isalnum(char);
int strlen(const char *);
int fgets(char *, std::ifstream&);
int strncpy(char *, const char *, int);

}

#endif
