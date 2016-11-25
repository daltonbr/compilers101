/*
 * Lexer.c
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#include <ctype.h>
#include <stdio.h>

#include "Tokens.h"
#include "Lexer.h"
#include "Keywords.h"

/*
 * lexeme definition
 */
char lexeme[MAX_ID_LENGTH + 1];
int lexcursor;
int line_count = 1;

int skip_comments(FILE * tape) {
	int head;

_skipspaces:
	while (isspace(head = getc(tape)))
		if(head == '\n') line_count++;

	if (head == '{') {
		while ((head = getc(tape)) != '}') {
			if(head == '\n') line_count++;
			if (head == EOF)
				return EOF;
		}
		goto _skipspaces;
	} else if(head == '(') {
		head = getc(tape);
		if(head != '*') {
			ungetc(head, tape);
			ungetc('(', tape);
		} else {
_keeplooking:
			while ((head = getc(tape)) != '*') {
				if(head == '\n') line_count++;
				if (head == EOF)
					return EOF;
			}
			if ((head = getc(tape)) == ')') {
				goto _skipspaces;
			} else {
				ungetc(head, tape);
				goto _keeplooking;
			}
		}
	}
	else {
		ungetc(head, tape);
	}

	return 0;
}

/*
 * ID = [A-Za-z][A-Za-z0-9_]*
 */
int is_ID(FILE * tape) {
	int out;
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);
	if (isalpha(lexeme[lexcursor])) {
		lexcursor++;
		while (isalpha(lexeme[lexcursor] = getc(tape))
				|| isdigit(lexeme[lexcursor]) || lexeme[lexcursor] == '_') {
			if (lexcursor < MAX_ID_LENGTH)
				lexcursor++;
		}
		ungetc(lexeme[lexcursor], tape);
		lexeme[lexcursor] = 0;

		if (out = is_keyword(lexeme)) {
			return out;
		}

		return ID;
	}
	ungetc(lexeme[lexcursor], tape);
	return 0;
}

int is_STRCONST(FILE * tape) {
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);

	if (lexeme[lexcursor] == '\'') {
		lexcursor++;
		while (1) {
			lexeme[lexcursor] = getc(tape);
			if(lexeme[lexcursor] == '\n') line_count++;

			if (lexeme[lexcursor] == '\'') {
				if (lexeme[lexcursor-1] != '\\') {
					lexcursor++;
					break;
				}
			}

			lexcursor++;
		}

		lexeme[lexcursor] = 0;
		return STRCONST;
	}

	ungetc(lexeme[lexcursor], tape);
	return 0;
}

/*
 * EXP = ('e'|'E')('+'|'-')?[0-9]+
 */
int is_EXP(FILE * tape) {
	int e = getc(tape),
		c;

	if (e == 'e' || e == 'E') {
		lexeme[lexcursor] = e;
		lexcursor++;

		c = getc(tape);

		if(c == '+' || c == '-') {
			lexeme[lexcursor] = c;
			lexcursor++;

			c = getc(tape);
		} else if (!isdigit(c)) {
			ungetc(c, tape);
			ungetc(e, tape);
			lexcursor--;
			return 0;
		}

		do {
			lexeme[lexcursor] = c;
			lexcursor++;

		} while(isdigit(c = getc(tape)));

		ungetc(c, tape);
		return 1;
	}

	ungetc(e, tape);
	return 0;
}

/*
 * (0 | [1-9][0-9]+) ('.'[0-9]*)? EXP?
 * */
int is_NUM(FILE * tape) {
	int numtype = 0;
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);

	if (isdigit(lexeme[lexcursor])) {
		numtype = UINT;

		if (lexeme[lexcursor] != '0') {
			lexcursor++;
			while (isdigit(lexeme[lexcursor] = getc(tape)))
				lexcursor++;
		} else {
			lexcursor++;
			lexeme[lexcursor] = getc(tape);
		}

		if (lexeme[lexcursor] == '.') {
			lexcursor++;

			//tratamento para array [1..2]
			lexeme[lexcursor] = getc(tape);
			if(lexeme[lexcursor] == '.') {
				ungetc(lexeme[lexcursor], tape);
				lexcursor--;
				ungetc(lexeme[lexcursor], tape);
				lexeme[lexcursor] = 0;
				return numtype;
			} else {
				ungetc(lexeme[lexcursor], tape);
			}

			numtype = FLOAT;

			while (isdigit(lexeme[lexcursor] = getc(tape)))
				lexcursor++;
		}

		//at this point, there can only be an invalid character read into lexeme
		ungetc(lexeme[lexcursor], tape);

		is_EXP(tape);

		lexeme[lexcursor] = 0;
		return numtype;
	}
	if (lexeme[lexcursor] == '.') {
		lexcursor++;
		if (isdigit(lexeme[lexcursor] = getc(tape))) {
			lexcursor++;
			while (isdigit(lexeme[lexcursor] = getc(tape)))
				lexcursor++;

			ungetc(lexeme[lexcursor], tape);
			is_EXP(tape);

			lexeme[lexcursor] = 0;
			return FLOAT;
		}
		ungetc(lexeme[lexcursor], tape);
		lexcursor--;
	}
	ungetc(lexeme[lexcursor], tape);
	return 0;
}

int is_LEQ_NEQ(FILE * tape) {
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);

	if (lexeme[lexcursor] == '<') {
		lexcursor++;
		lexeme[lexcursor] = getc(tape);
		if(lexeme[lexcursor] == '=') {
			lexcursor++;
			lexeme[lexcursor] = 0;
			return LEQ;
		} else if (lexeme[lexcursor] == '>') {
			lexcursor++;
			lexeme[lexcursor] = 0;
			return NEQ;
		} else {
			ungetc(lexeme[lexcursor], tape);
			lexcursor--;
		}
	}

	ungetc(lexeme[lexcursor], tape);
	return 0;
}

int is_GEQ(FILE * tape) {
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);

	if (lexeme[lexcursor] == '>') {
		lexcursor++;
		lexeme[lexcursor] = getc(tape);
		if(lexeme[lexcursor] == '=') {
			lexcursor++;
			lexeme[lexcursor] = 0;
			return GEQ;
		} else {
			ungetc(lexeme[lexcursor], tape);
			lexcursor--;
		}
	}

	ungetc(lexeme[lexcursor], tape);
	return 0;
}

int is_ASSIGNMENT(FILE * tape) {
	lexcursor = 0;
	lexeme[lexcursor] = getc(tape);

	if (lexeme[lexcursor] == ':') {
		lexcursor++;
		lexeme[lexcursor] = getc(tape);
		if(lexeme[lexcursor] == '=') {
			lexcursor++;
			lexeme[lexcursor] = 0;
			return ASSIGNMENT;
		} else {
			ungetc(lexeme[lexcursor], tape);
			lexcursor--;
		}
	}

	ungetc(lexeme[lexcursor], tape);
	return 0;
}

int get_token(FILE * tape) {
	int token;

	if (token = skip_comments(tape))
		return token;

	if (token = is_ID(tape))
		return token;

	if (token = is_NUM(tape))
		return token;

	if (token = is_STRCONST(tape))
		return token;

	if (token = is_LEQ_NEQ(tape))
		return token;

	if (token = is_GEQ(tape))
		return token;

	if (token = is_ASSIGNMENT(tape))
		return token;

	//keep in lexeme the correct content
	lexcursor = 0;
	lexeme[lexcursor] = token = getc(tape);
	lexcursor++;
	lexeme[lexcursor] = 0;
	return token;
}
