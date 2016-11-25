#include <string.h>
#include "Lexer.h"
#include "Symtab.h"
#include "ErrorHandler.h"

char            symtab_names[MAX_SYMBOLS * (MAX_ID_LENGTH + 1)];
int             symtab_names_next_entry = 0;
/**
 * symtab:
 *          0         1
      |id_position|datatype|
**/

int             symtab_descriptor[MAX_SYMBOLS][2];
int             symtab_next_entry = 0;

int symtab_lookup(char const *symbol)
{
    int             i;
    for (i = symtab_next_entry - 1; i > -1; i--) {
        if (strcmp(symtab_names + symtab_descriptor[i][ID_POS], symbol)
            == 0)
            return i;
    }
    return -1;
}

int symtab_append(const char *symbol)
{
    int             j = symtab_next_entry;
    int             entry = symtab_lookup(symbol);
    if (entry == -1) {
    	if(symtab_next_entry >= MAX_SYMBOLS) {
    		fatal_error(SYMBOL_OVERFLOW, NULL);
    	}

        strcpy(&symtab_names[symtab_names_next_entry], symbol);
        symtab_descriptor[symtab_next_entry][ID_POS] =
            symtab_names_next_entry;
        symtab_names_next_entry += strlen(symbol) + 1;
        symtab_next_entry++;
    } else {
        fatal_error(SYMBOL_OVERLAP, NULL);
        return -1;
    }
    return j;
}

void symtab_settype(int first, int last, int buitintype)
{
    int             i;
    for (i = first; i <= last; i++) {
        symtab_descriptor[i][DATYPE] = buitintype;
    }
}
