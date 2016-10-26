/**@<keywords.h>::**/

/*enum {	IF = 0x4097, THEN, ELIF, ELSE, ENDIF, WHILE, DO, DONE};*/

enum{
	BEGIN = 0x4097, IF, THEN, ELSE, WHILE, DO, REPEAT, UNTIL, PROCEDURE, FUNCTION, VAR, INTEGER, REAL, BOOLEAN, DIV, MOD, AND, OR, NOT, END
};

extern char *keywords[];
extern int iskeyword(char const *);
