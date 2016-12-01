/**@<parser.h>::**/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STATEMENT       0
#define EXPRESSION      1

extern int lookahead;
extern char lexeme[];
extern int      lineno;
extern int gettoken(FILE *);
extern FILE *target;

extern int      loopcount,
                constcount;

void
match(int);
void assemblyrelop(int);
void mypas(void);
void start(void);
void programID(void);
void declarations(void);
void varspecs(void);
void typespec(int, int, int);
void idvalue(void);
int smptype(void);
void exprlist(int argc, int argt[]);
void sbpspecs(void);
void funspec(void);
void procedspec(void);
int parmspec(void);
void parmlist(void);
int formalparm(void);
void blockstmt(void);
void stmtlist(void);
void stmt(void);
void ifstmt(void);
int expr(void);
int isrelop(void);
void simpexpr(void);
int isnegate(void);
int isaddop(void);
int term(void);
void repstmt(void);
int ismulop(void);
int factor(void);
int   expression(int);
void whilestmt(void);
void repstmt(void);
int idstmt(int);
void idvalue(void);
