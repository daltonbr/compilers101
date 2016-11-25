/*
 * Keywords.h
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#ifndef KEYWORDS_H_
#define KEYWORDS_H_

extern char *keyword[];

int is_keyword (char *);

enum {
    BEGIN = 1024,
    FOR,
    TO,
    DOWNTO,
    DO,
    WHILE,
    REPEAT,
    UNTIL,
    IF,
    THEN,
    ELSE,
    AND,
    OR,
    NOT,
    DIV,
    MOD,
    VAR,
    PROGRAM,
    INTEGER,
    REAL,
    DOUBLE,
    BOOLEAN,
    TRUE,
    FALSE,
    END,
};

#endif /* KEYWORDS_H_ */
