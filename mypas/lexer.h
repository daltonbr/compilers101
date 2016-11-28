/**@<lexer.h>::**/

extern char *keywords[];
extern int iskeyword (char const *identifier);
#define MAXID_SIZE 32
extern char lexeme[MAXID_SIZE+1];
