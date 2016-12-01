#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tabsym.h"

extern FILE *out;
extern FILE *outtmp;

typedef enum actions{
  NOACTION,
  EXIT,
  MARK,
  READ,
  WRITE,
  OPEN,
  CLOSE,
  CONSTANTES = 200,
  VARIAVEIS,
  ATRIBUIR,
  SECTION_TEXT,
  SECTION_RODATA,
  PRINT_CONST_LEX,
  PRINT_CONST,
  SECTION_BSS,
  PRINT_VAR,
  JMP_START,
  ACT_THEN,
  ACT_ELSE,
  PUT_LABEL_IF,
  PUSH,
  POP,
  POP2Acc,
  MULTIPLICA,
  DIVIDE_FLOAT,
  DIVIDE_INT,
  DIVIDE_RESTO,
  LOGIC_AND,
  PLUS,
  MINUS,
  LOGIC_OR,
  IGUAL,
  DIFERENTE,
  MENOR_QUE,
  MENOR_IGUAL_QUE,
  MAIOR_IGUAL_QUE,
  MAIOR_QUE,
  PERTENCE,
  SALVE_PROC,
  CALL_PRO,
  PUT_BUFFER_OUT,
  PUT_BUFFER_IN,
  NEGAR,
  INC_PRINT_LABEL_CASE,
  EXPRESSION_CASE,
  SALVE_VARIABLE,
  VERIFICA_CONTADOR,
  INCONDITIONAL_JMP,
  PRINT_LABEL,
  PRINT_LITERAL_STATEMENTS,
  OPOSTO
}action_t;

void semantic(action_t action);
void pop(const char *address);
void push(const char *address);
/* if-then-else */
void act_then(int counter_label_if);
void act_else(int counter_label_if, int END_L);
void put_label_if(int counter_label_if);
/* case */
void print_conditional_jmp_label_case(int l);
void jmp_end_label_case(int l);
void print_next_label_case(int l);
void put_end_label_case(int l);

/* while */
void put_label_while(int l);
void jmp_to_begin(int l);
void jmp_when_false(int l);
void put_end_while_case(int l);

/* repeat */
void print_label_repeat(int l);
void jmp_if_false(int l);

/* for */
void put_label_for(int l);
void verifica_contador(const char *s, int l);
void jmp_label_for(const char *s, int l);
#endif