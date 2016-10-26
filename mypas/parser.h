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
void dostmt(void);
void expr (void);
void rest (void);
void term (void);
void quoc (void);
void fact (void);
int addop (void);
int mulop (void);
