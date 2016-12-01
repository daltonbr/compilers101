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
    WRITE,
    VAR, //524300
    BOOLEAN, //524301
    INTEGER, //524302
    REAL, //524303
    DOUBLE, //524304
    DIV, //524305
    MOD, //524306
    AND, //524307
    OR, //524308
    NOT, //524309
    TRUE, //524310
    FALSE, //524311
	PROGRAM, //524312
    END //524313
};

extern char *keywords[];
extern int iskeyword(char const *);
