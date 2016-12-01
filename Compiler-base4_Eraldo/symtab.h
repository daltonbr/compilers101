#define                 MAXARGNUM       32

#define OBJSVAR 0
#define OBJSPAR 1
#define OBJRPAR 2
#define OBJFUNC 3
#define OBJPROC 4

typedef struct _symbol_table_ {
    char            symbol[MAXIDLEN + 1];
    int             lexlevel /* 0 if global,  1 local */ ;
    int             objtype;
    /*
     * = 0 => simple variable
     * = 1 => simple parm
     * = 2 => reference parm
     * = 3 => function
     * = 4 => procedure (datatype = 0) 
     */
    int             argc;
    int             argt[MAXARGNUM];
    int             parmframe;
    int             dtatype;
    char            offset[MAXIDLEN + 1];
} SYMTAB;

extern SYMTAB   symtab[MAXTBENTRIES];

extern int      symtab_nextentry,
                lexlevel,
                last_local_offset,
                last_parm_offset;
extern int      argc,
                argt[MAXARGNUM];


int             symtab_add(char const *newsymbol);
int             symtab_lookup(char const *query);
