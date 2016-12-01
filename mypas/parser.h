/**@<parser.h>::**/

extern int lookahead; // @ local
extern char lexeme[];
extern int gettoken (FILE *); // @ lexer.c
extern char *symtab_stream;  // @ symtab.c

extern FILE *source, *object; // @ main.c

extern void match (int); // @ local

/* syntax names */
void mypas(void);
void programhead(void);
void body(void);
void smtmlist(void);
void cmd(void);
void ifstmt(void);
void whilestmt(void);
void repeatstmt(void);
void dostmt(void);
void blockstmt(void);
void stmt(void);
void stmtlist(void);
int expr (int inherited_type);
int superexpr (int inherited_type);
int vartype(void);
void rest (void);
void term (void);
void quoc (void);
void fact (void);
int addop (void);
int mulop (void);
void parmdef(void);
void fnctype(void);
void parmdef(void);
void fnctype(void);
int iscompatible(int, int);
int isrelop();
void declarative(void);
void imperative(void);
char ** namelist(void);
void writestmt(void);
