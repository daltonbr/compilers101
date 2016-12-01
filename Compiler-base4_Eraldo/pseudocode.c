#include <stdio.h>
#include <stdlib.h>
#include <pseudocode.h>
#include <typecheck.h>
#include <keywords.h>

int             loopcount = 1,
    constcount = 1;

FILE           *object;

void
gotolabel(int lblidx)
{
    fprintf(object, "\tjmp .L%i\n", lblidx);
}

void
gofalse(int lblidx)
{
    fprintf(object, "\tandl %%eax,%%eax\n");
    fprintf(object, "\tjz .L%i\n", lblidx);
}

void
assemblyrelop(int condition)
{
      /*
     * 1 -> <
     * 2 -> >
     * 3 -> <>
     * 4 -> =
     * 5 -> <=
     * 6 -> >=
     */
  int lblend = loopcount, lblfalse = loopcount + 1;
  loopcount += 2;
  fprintf(object, "\tcmpl %%eax,(%%esp)\n");
  switch(condition){
    case 1:
      fprintf(object, "\tjge ");break;
    case 2:
      fprintf(object, "\tjle ");break;
    case 3:
      fprintf(object, "\tje ");break;
    case 4:
      fprintf(object, "\tjne ");break;
    case 5:
      fprintf(object, "\tjg ");break;
    default:
      fprintf(object, "\tjl ");
  }
  fprintf(object, ".L%i\n", lblfalse);
  fprintf(object, "\txorl %%eax,%%eax\n");
  fprintf(object, "\tnegl %%eax\n");
  gotolabel(lblend);
  mklbl(lblfalse);
  fprintf(object, "\txorl %%eax,%%eax\n");
  mklbl(lblend);
}

void
do_operation(int op, int oprndtype)
{
  switch(oprndtype){
    case TYPEINT:
      switch (op) {
        case '+':
          fprintf(object, "\taddl %%eax,(%%esp)\n");
          break;
        case '-':
          fprintf(object, "\tsubl %%eax,(%%esp)\n");
          break;
        case '*':
          fprintf(object, "\timull (%%esp)\n");
          break;
        case DIV:
          fprintf(object, "\tcltd\n\tidivl (%%esp)\n");
          break;
        case MOD:
          fprintf(object, "\tcltd\n\tidivl (%%esp)\n");
          fprintf(object, "\tmovl %%edx,%%eax\n");
          break;
        case '/':
          break;
      }
      break;
    case TYPEFLT:
      break;
    case TYPEDBL:
      break;
    case TYPEASC:
      break;
    case TYPESTR:
      break;
    case TYPELOG:
      /*
       * case OR: case AND: ; 
       */
      ;
  }
}

void
rvalue(char const *offset)
{
    fprintf(object, "\tmovl %s,%%eax\n",offset);
}

void
lvalue(char const *offset)
{
  fprintf(object, "\tmovl %%eax,%s\n", offset);
}

void
assemblyconst(char const *strconst,int constype)
{
  double rconst;
  char c;
  switch(constype){
  case TYPEINT:
    fprintf(object, "\tmovl $%d,%%eax\n", atoi(strconst));
    break;
  case TYPEFLT:
    rconst = atof(strconst);
    fprintf(object, "\tmovl $%u,%%eax\n", ((unsigned *)&rconst)[1]);
    break;
  case TYPEDBL:
    rconst = atof(strconst);
    fprintf(object, "\tmovl $%u,%%eax\n", ((unsigned *)&rconst)[0]);
    fprintf(object, "\tmovl $%u,%%edx\n", ((unsigned *)&rconst)[1]);
    break;
  case TYPEASC:
    c = strconst[1];
    fprintf(object, "\tmovl $%hd,%%eax\n", c);
    break;
  case TYPESTR:
  case TYPELOG:
  ;
  }
}

void
mklbl(int lblidx)
{
    fprintf(object, ".L%i:\n", lblidx);
}

void
push(int size)
{
      switch(size){
      case 4:
          fprintf(object, "\tpush %%eax\n");
          break;
      case 8:
          fprintf(object, "\tpush %%edx\n");
          fprintf(object, "\tpush %%eax\n");
          break;
    }
}

void
pop(int size)
{
      switch(size){
      case 4:
          fprintf(object, "\tpop %%eax\n");
          break;
      case 8:
          fprintf(object, "\tpop %%eax\n");
          fprintf(object, "\tpop %%edx\n");
          break;
    }
}
