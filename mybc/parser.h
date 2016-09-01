/**@<parser.h>::**/

extern int lookahead; // @ local

extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

extern void add_to_stack (); // @ interpreter
extern void next_value (double value); // @ interpreter
extern void next_value_with_id (char *); // @ interpreter
extern void operation (char); // @ interpreter

/* syntax names */
void expr (void);
void rest (void);
void term (void);
void quoc (void);
void fact (void);
int addop (void);
int mulop (void);
