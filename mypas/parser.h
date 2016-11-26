/**@<parser.h>::**/

extern int lookahead; // @ local
extern char lexeme[];
extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

/* syntax names */
void mypas(void);
void body(void);
void smtmlist(void);
void cmd(void);
void ifstmt(void);
void whilestmt(void);
void repeatstmt(void);
void dostmt(void);
void expr (int inherited_type);
int superexpr (int inherited_type);
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
void declarative(void);
void imperative(void);
char ** namelist(void);
