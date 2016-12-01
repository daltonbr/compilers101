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
    WRITE, //524300
    VAR, //524301
    BOOLEAN, //524302
    INTEGER, //524303
    REAL, //524304
    DOUBLE, //524305
    DIV, //524306
    MOD, //524307
    AND, //524308
    OR, //524309
    NOT, //524310
    TRUE, //524311
    FALSE, //524312
	PROGRAM, //524313
    END //524314
};

extern char *keywords[];
extern int iskeyword(char const *);
