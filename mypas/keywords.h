/**@<keywords.h>::**/

enum{
	BEGIN = 0x80000, IF, THEN, ELIF, ELSE, ENDIF, WHILE, DO, REPEAT, UNTIL, PROCEDURE, FUNCTION, VAR, BOOLEAN, INTEGER, REAL, DOUBLE, DIV, MOD, AND, OR, NOT, TRUE, FALSE, END
};

extern char *keywords[];
extern int iskeyword(char const *);
