#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tokens.h>
#include <symtab.h>

SYMTAB          symtab[MAXTBENTRIES];

int             symtab_nextentry;

int             lexlevel = 0;

int             last_local_offset = 0;

int             last_parm_offset = 8;

int             argc,
                argt[MAXARGNUM];

int
symtab_add(char const *newsymbol)
{
    int             i;
    if ((i = symtab_lookup(newsymbol)) < 0
        || symtab[i].lexlevel < lexlevel) {
        if (symtab_nextentry < MAXTBENTRIES) {
            strcpy(symtab[symtab_nextentry].symbol, newsymbol);
            symtab[symtab_nextentry].lexlevel = lexlevel;
            return ++symtab_nextentry;
        } else {
            fprintf(stderr,"maximum symbol entries exceeded... exiting\n");
            exit(-4);
        }
    } else {
        fprintf(stderr,"%s already declared in the same lexical level... exiting\n",newsymbol);
        exit(-3);
    }
}

int
symtab_lookup(char const *query)
{
    int i;
    for (i = symtab_nextentry - 1; i > -1; i--) {
        if (strcmp(symtab[i].symbol, query) == 0)
            break;
    }
    return i;
}
