#include <stdlib.h>
#include "error.h"

/* variáveis */
int line = 1, col = 0;

char *error_msg[COUNT]={
  "No error",
  "Unknown symbol",
  "Expected a '''",
  "Expected a '\"'",
  "Expected a number"
  "Expected an integer value",
  "Expected an double value",
  "Expected a literal (char or string)",
  "Expected an operator",
  "Expected an ':='",
  "Expected an ':=' or an operator",
  "Expected a term",
  "Expected a '.'",
  "Expected a ':'"
  "Expected a ','",
  "Expected a ';'",
  "Expected a '('",
  "Expected a ')'",
  "Expected a '['",
  "Expected a ']'",
  "Expected a '='",
  "Expected an identifier",
  "Expected a 'PROGRAM'",
  "Expected a 'BEGIN'",
  "Expected a 'END'",
  "Expected a 'IF'",
  "Expected a 'THEN'",
  "Expected a 'ELSE'",
  "Expected a 'CASE'",
  "Expected a 'OF'",
  "Expected a 'LABEL'",
  "Expected a 'CONST'",
  "Expected a 'TYPE'",
  "Expected a 'VAR'",
  "Expected a 'WHILE'",
  "Expected a 'DO'",
  "Expected a 'REPEAT'",
  "Expected a 'UNTIL'",
  "Expected a 'FOR'",
  "Expected a 'TO'",
  "Expected a 'GOTO'",
  "Divizion by zero",
  "Uncompatible type of identifier",
  "Syntax error"
};


/* funções */
void error(erro_t err){
  printf("\nError (%d,%d): %s.\n",line,col,error_msg[err]);
  switch(err){
  case NONE: case UNKNOWN:
    ;
    break;
  default:
    exit(1);
  }
}
