/**@<parser.h>::**/

#define STATEMENT       0
#define EXPRESSION      1

extern char     lexeme[];
extern int      lookahead;
extern int      lineno;
extern int      gettoken(FILE *);
extern FILE    *target;

void            blockstmt(void);
void            stmt(void);
void            match(int);
void            mypas(void);
void            programID(void);
void            declarations(void);
void            varspecs(void);
int             smptype(void);
void            typespec(int, int, int);
int             idstmt(int);
int             expression(int);
void            exprlist(int argc, int argt[]);
void            sbpspecs(void);
int             expr(void);
int             term(void);
int             factor(void);
void            ifstmt(void);
void            whilestmt(void);
void            repstmt(void);
int             formalparm(void);
int             parmpecs(void);
