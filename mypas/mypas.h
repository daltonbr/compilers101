/**@<mypas.h>::**/

extern FILE *source, *object;
extern int lookahead; // @ parser.c
extern int gettoken(FILE *);
extern void mypas(void);
