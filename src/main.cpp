#include <cstdio>
#include <cassert>
#include <cstring>

#include "include/lexer.h"
#include "include/parser.h"
#include "include/main.h"
#include "include/codegen.h"

int main(int argc, const char **argv)
{
  if(argc < 2)
  {
    usage(argv[0]);
    return 1;
  }

  flags params = {0};
  if(parse_flags(&params, argc, argv))
  {
    // Failed to parse parameters.
    return 1;
  }

  if(params.help_menu)
  {
    usage(argv[0]);
  }
  else
  {
    int ret = compile_file(argv[1], &params);
    printf("Compiler Return Code: %d\n", ret);
  }

  free_parameters_fields(&params);

  return 0;
}



/**********************************/

/*
 * parse_flags() does not initialize flags structure!
 * Its on you to initialize the structure.
 */
bool parse_flags(flags *parameters, int argc, const char **argv)
{
  bool error = false;
  for(int i = 1; i < argc ; i++)
  {
    if(match_param(argv[i], "--dump-ast"))
    {
      parameters->dump_ast = true;
    }
    if(match_param(argv[i], "--output"))
    {
      if(i + 1 == argc)
      {
        // This is last flag in input, and there is no file name.
        error = true;
        parameters->output = nullptr;
        printf("Output file is not specified\n");
        break;
      }
      i++;
      unsigned int out_file_size = my::strlen(argv[i]);
      parameters->output = new char[out_file_size + 1]();
      memcpy(parameters->output, argv[i], out_file_size + 1);
    }
    if(match_param(argv[i], "--dump-tokens"))
    {
      parameters->dump_tokens = true;
    }
    if(match_param(argv[i], "--dry-run"))
    {
      parameters->dry_run = true;
    }
    if(match_param(argv[i], "--help"))
    {
      parameters->help_menu = true;
    }
  }
  return error;
}

void free_parameters_fields(flags *parameters)
{
  delete[] parameters->output;
}

bool match_param(const char *argv, const char *match)
{
  for(; *argv != '\0' && *match != '\0'; argv++, match++)
  {
    if(*argv != *match)
    {
      return false;
    }
  }
  if(*argv == '\0' && *match == '\0')
  {
    return true;
  }
  return false;
}

bool open_file(std::ifstream *file,const char *filename)
{
  file->open(filename);
  if(file->is_open())
  {
    return true;
  }
  printf("File '%s' was not open(maybe there is no such file)\n", filename);
  return false;
}

void usage(const char *progname)
{
  printf("Usage: %s <file> [flag(s)]\n", progname);
  printf("\nFLAGS:\n");
  printf("\t--dump-ast : Print parsed AST.\n");
  printf("\t--dump-tokens : Print parsed tokens.\n");
  printf("\t--dry-run : Run compiler in dry mode.\n");
  printf("\t--help : Print this menu.\n");
  printf("\t--output <file> : Specify output file name.\n");
}

void dump_tokens(std::vector<token> *tokens)
{
  printf("\n---Dump Tokens Start---\n");
  token *ptr = tokens->data();
  int size = tokens->size();
  for(int i = 0; i < size; i++)
  {
    if(ptr[i].type == token_type::INTLIT)
    {
      printf("Type: %s, Value: %d\n", tok2string(ptr[i].type), ptr[i].intval);
    }
    else
    {
      printf("Type: %s\n", tok2string(ptr[i].type));
    }
  }
  printf("---Dump Tokens End---\n\n");
}

int compile_file(const char *inputfile, flags *parameters)
{

  std::ifstream file;
  if(!open_file(&file, inputfile))
  {
    return 1;
  }


  // Scanning file to get vector of tokens
  Lexer lexer(&file);
  std::vector<token> *tokens = scan(&lexer);
  file.close();

  // When nullptr is returned, was_error should always be true,
  // but anyway additional check
  if(lexer.was_error)
  {
    // Asserting to ensure that when error in occured,
    // nullptr is always returned.
    // If nullptr is not returned, then possible there is a memory leak.
    assert(tokens == nullptr);
    printf("Occured error while scanning\n");
    return 1;
  }

  if(parameters->dump_tokens)
  {
    dump_tokens(tokens);
  }

  Parser parser(tokens->data());
  ast_node *root = parser.add_expression();
  delete tokens;

  // Now tokens are parsed. We dont need vector anymore.
  if(parser.was_error)
  {
    assert(root == nullptr);
    printf("Occured error while parsing\n");
    return 1;
  }

  // Serve --dump-ast flag.
  if(parameters->dump_ast)
  {
    printf("\n---Dump AST Start---\n");
    print_ast(root, 0);
    printf("---Dump AST End---\n\n");
  }

  if(parameters->dry_run)
  {
    printf("Executed with dry run parameter, code generation aborted.\n");
  }
  else
  {
    code_gen(root, parameters);
  }

  free_ast(root);

  return 0;
}
