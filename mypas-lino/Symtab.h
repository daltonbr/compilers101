#include "Lexer.h"
#define MAX_SYMBOLS 0x1000
enum {
    ID_POS = 0,
    DATYPE,
};
extern int      symtab_descriptor[MAX_SYMBOLS][2];
extern int      symtab_next_entry;
int             symtab_lookup(char const *symbol);
int             symtab_append(char const *symbol);
void            symtab_settype(int, int, int);
