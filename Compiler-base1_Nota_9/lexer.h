#ifndef LEXER_H
#define LEXER_H

/* includes */
#include <ctype.h>
#include "error.h"
#include "semantic.h"

/* vari�veis */
extern FILE *in;
int show_heap;/* variável utilizada pelo basicalc e o semantic */

/* prot�tipos */
int gettoken(void);
void match(int token, action_t action, erro_t err);

#endif
