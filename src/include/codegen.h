#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "main.h"


enum class reg
{
  R8, R9, R10, R11
};

reg allocate_register();
void free_register(reg regis);
void init_registers();

void code_gen(ast_node *root, flags *parameters);
void ast_asm_gen(ast_node *root, int fd);

reg asm_load_val(int intval, int fd);
reg asm_gen_expression(ast_node *root, int fd);



#endif /* CODEGEN_H */
