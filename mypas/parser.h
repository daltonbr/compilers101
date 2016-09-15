/**@<parser.h>::**/

extern int lookahead; // @ local

extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

/* syntax names */
void progcalc(void);
void cmdblock(void);
int cmdsep(void);
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
