/*
 * Keywords.c
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#include <string.h>
#include "Keywords.h"
#include "Tokens.h"

char *keyword[] = {
    "begin",
    "for",
    "to",
    "downto",
    "do",
    "while",
    "repeat",
    "until",
    "if",
    "then",
    "else",
    "and",
    "or",
    "not",
    "div",
    "mod",
    "var",
    "program",
    "integer",
    "real",
    "double",
    "boolean",
    "true",
    "false",
    "end",
};

int is_keyword(char *identifier) {
	int i;
	for (i = BEGIN; i <= END; i++) {
		if (strcasecmp(identifier, keyword[i - BEGIN]) == 0) {
			return i;
		}
	}
	return 0;
}
