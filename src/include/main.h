#ifndef MAIN_H
#define MAIN_H

#include <fstream>

struct flags
{
  bool dump_ast;
  char *output;
};


void usage(const char *progname);
int compile_file(const char *inputfile, flags *params);
bool open_file(std::ifstream *file,const char *filename);
bool match_param(const char *argv, const char *match);

bool parse_flags(flags *parameters, int argc, const char **argv);
void free_parameters_fields(flags *parameters);

#endif /* MAIN_H */
