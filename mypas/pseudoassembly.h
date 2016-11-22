int lmovel(char const *variable);
/*unified label counter*/
extern int labelcounter;
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
int addlog(void);
int addint(void);
int addflt(void);
int adddbl(void);
int sublog(void);
int subint(void);
int subflt(void);
int subdbl(void);
/*binary multiplication and inverse*/
int mullog(void);
int mulint(void);
int mulflt(void);
int muldbl(void);
int divint(void);
int divflt(void);
int divdbl(void);
