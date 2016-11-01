/**@<symtab.h>::**/

#define MAX_SYMTAB_ENTRIES 0x10000

extern int symtab[MAX_SYMTAB_ENTRIES][2];
int symtab_append(char const *name, int type);
int symtab_lookup(char const *name);

extern char *symtab_stream;
