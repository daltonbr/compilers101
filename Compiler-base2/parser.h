/**@<parser.h>::**/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int lookahead;
//int gettoken(FILE *target);
extern FILE *target;
void match(int expected);

void mypas(void);
void programID(void);
void declarations(void);
void varspecs(void);
void typespec(void);
void idstmt(void);
void idvalue(void);
void exprlist(void);
void sbpspecs(void);
void funspec(void);
void procedspec(void);
void parmlist(void);
void formparm(void);
void blockstmt(void);
void stmtlist(void);
void stmt(void);
void ifstmt(void);
void expr(void);
//int isrelop(void);
void simpexpr(void);
int isnegate(void);
//int isaddop(void);
void term(void);
//int ismulop(void);
void factor(void);
void whlstmt(void);
void repstmt(void);


