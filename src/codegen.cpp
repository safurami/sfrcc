#include <cstdio>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

#include "include/codegen.h"


static bool free_regs[4];
static const char *registers[] = {"r8", "r9", "r10", "r11"};

void init_registers()
{
  free_regs[0] = free_regs[1] = free_regs[2] = free_regs[3] = 1;
}

reg allocate_register()
{
  for(int i = 0; i < 4; i++)
  {
    if(free_regs[i])
    {
      free_regs[i] = 0;
      return (reg)i;
    }
  }
  printf("Out of registers!\n");
  assert(false);
}

void free_register(reg regis)
{
  free_regs[(int)regis] = 1;
}

void code_gen(ast_node *root, flags *parameters)
{
  int fd;
  if(parameters->output)
  {
    fd = open(parameters->output, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  }
  else
  {
    fd = open("output.s" , O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  }
  init_registers();
  asm_gen_expression(root, fd);
  close(fd);
}

void ast_asm_gen(ast_node *root, int fd)
{
}

reg asm_gen_expression(ast_node *root, int fd)
{
  reg left, right;
  switch(root->tag)
  {
    case ast_node::LITERAL:
      return asm_load_val(root->data.literal.intval, fd);
    case ast_node::BINARY:
      left = asm_gen_expression(root->data.binary.left, fd);
      right = asm_gen_expression(root->data.binary.right, fd);
      switch(root->data.binary.op)
      {
        case token_type::PLUS:
          dprintf(fd, "\tadd %s, %s\n", registers[(int)left], registers[(int)right]);
          free_register(right);
          return left;
        case token_type::MINUS:
          dprintf(fd, "\tsub %s, %s\n", registers[(int)left], registers[(int)right]);
          free_register(right);
          return left;
        case token_type::STAR:
          dprintf(fd, "\tmov rax, %s\n", registers[(int)left]);
          free_register(left);
          dprintf(fd, "\tmul %s\n", registers[(int)right]);
          dprintf(fd, "\tmov %s, rax\n", registers[(int)right]);
          return right;
        case token_type::SLASH:
          dprintf(fd, "\tmov rax, %s\n", registers[(int)left]);
          free_register(left);
          dprintf(fd, "\tdiv %s\n", registers[(int)right]);
          dprintf(fd, "\tmov %s, rax\n", registers[(int)right]);
          return right;
        default:
          assert(false);
      }
    default:
      assert(false);
  }
}

reg asm_load_val(int intval, int fd)
{
  reg free_reg = allocate_register();
  dprintf(fd, "\tmov %s, %d\n", registers[(int)free_reg], intval);
  return free_reg;
}
