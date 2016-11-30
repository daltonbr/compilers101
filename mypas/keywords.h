/**@<keywords.h>::**/

enum{
	BEGIN = 0x80000, //524288
    IF, //524289
    THEN, //524290
    ELIF, //524291
    ELSE, //524292
    ENDIF, //524293
    WHILE, //524294
    DO, //524295
    REPEAT, //524296
    UNTIL, //524297
    PROCEDURE, //524298
    FUNCTION, //524299
    VAR, //524300
    BOOLEAN, //524301
    INTEGER, //524302
    REAL,
    DOUBLE,
    DIV,
    MOD,
    AND,
    OR,
    NOT,
    TRUE,
    FALSE,
    END
};

extern char *keywords[];
extern int iskeyword(char const *);
