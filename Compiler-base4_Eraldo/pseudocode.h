void            gotolabel(int);
void            gofalse(int);
void            do_operation(int oper, int oprndtype);
void            rvalue(char const *);
void            memrecall(char const *label);
void            lvalue(char const *label);
void            mklbl(int lblidx);
void            assemblyconst(const char *strconst, int constype);
void            push(int);
void            pop(int);
void            assemblyrelop(int condition);

extern int      loopcount,
                constcount;
