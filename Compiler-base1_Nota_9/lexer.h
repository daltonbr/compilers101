#ifndef LEXER_H
#define LEXER_H

/* includes */
#include <ctype.h>
#include "error.h"
#include "semantic.h"

/* variáveis */
extern FILE *in;
int show_heap;/* variÃ¡vel utilizada pelo basicalc e o semantic */

/* protótipos */
int gettoken(void);
void match(int token, action_t action, erro_t err);

#endif
