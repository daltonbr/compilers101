/**@<pseudoassembly.h>::**/

extern FILE *object; // @ mypas.c
/*
void headerprog(char *nameProgram);
void rotuloprog(char *nameProgram);
*/
int lmovel(char const *variable);
int lmoveq(char const *variable);
int rmovel(char const *variable);
int rmoveq(char const *variable);

int writeln(char *strwrite);

/*unified label counter*/
    extern int labelcounter; // global var to label in machine code @ parser.c

/*control pseudo-instructions*/
    int gofalse(int label);
    int jump(int label);
    int mklabel(int label);

/*ULA pseudo-instructions*/
    /*unary*/
        int neglog(void);
        int negint(void);
        int negflt(void);
        int negdbl(void);
    /*binary addition and inverse*/
        int addlog(void);  // OR
        int addint(void);
        int addflt(void);
        int adddbl(void);
        int sublog(void);
        int subint(void);
        int subflt(void);
        int subdbl(void);
    /*binary multiplication and inverse*/
        int mullog(void);  // AND
        int mulint(void);
        int mulflt(void);
        int muldbl(void);
        int divint(void);
        int divflt(void);
        int divdbl(void);
