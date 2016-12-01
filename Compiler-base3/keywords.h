/**@<keywords.h>::**/


extern char    *keyword[];

int             iskeyword(char const *identifier);

enum {
  BEGIN = 0x10000,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    REPEAT,
    UNTIL,
    DIV,
    MOD,
    OR,
    AND,
    NOT,
    TRUE,
    FALSE,
    PROGRAM,
    VAR,
    INTEGER,
    REAL,
    DOUBLE,
    CHAR,
    TEXT,
    BOOLEAN,
    ARRAY,
    OF,
    PROCEDURE,
    FUNCTION,
    END
};

