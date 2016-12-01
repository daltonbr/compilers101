#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

/* definições */
typedef enum erro{
  NONE,        /* nenhum erro */
  UNKNOWN,     /* símbolo desconhecido */
  EXP_APOST_SIMPLES,
  EXP_APOST_DUPLO,
  EXP_NUMBER,  /* espera por um número */
  EXP_INT,		/* esperado inteiro  */
  EXP_DOU,     /* esperado double   */
  EXP_LITERAL,	/*  */
  EXP_OPERATOR,/* esperado operador */
  EXP_ASSIGN,  /* esperado ':='     */
  EXP_ASG_OPER,/* esperado ':=' ou operador */
  EXP_TERM,		/* um termo era experado     */
  EXP_PTO,		/* um ponto era esperado     */
  EXP_PTOPTO,  /* esperado ':' */
  EXP_VIRG,		/* esperado ',' */
  EXP_PTOVIRG, /* esperado ';' */
  EXP_ABRPAR,	/* esperado '(' */
  EXP_FECPAR,  /* esperado ')' */
  EXP_ABRCOL,  /* esperado '[' */
  EXP_FECCOL,  /* esperado ']' */
  EXP_IGUAL,   /* esperado '=' */
  EXP_ID,		/* esperado identificador */
  EXP_PROGRAM,
  EXP_BEGIN,
  EXP_END,
  EXP_IF,
  EXP_THEN,
  EXP_ELSE,
  EXP_CASE,
  EXP_OF,
  EXP_LABEL,
  EXP_CONST,
  EXP_TYPE,
  EXP_VAR,
  EXP_WHILE,
  EXP_DO,
  EXP_REPEAT,
  EXP_UNTIL,
  EXP_FOR,
  EXP_TO,
  EXP_GOTO,
  DIV_BY_ZERO,  /* tentativa de divisÃ£o por zero */
  TYPE_INCOMPATIBLE,
  SINTAX_ERR,   /* erro de sintáxe */
  COUNT         /* quantidade de erros */
}erro_t;

/* variáveis */
extern int line;
extern int col;

/* protótipos */
void error(erro_t err);

#endif
