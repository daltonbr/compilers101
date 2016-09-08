/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <keywords.h>

#define MAXID_SIZE	32
char lexeme[MAXID_SIZE+1];

void skipspaces (FILE *dish)
{
        int lexeme[0];

        while ( isspace ( lexeme[0] = getc (dish) ) );

        ungetc ( lexeme[0], dish );
}

int is_identifier(FILE *dish)
{
	int i;

        int lexeme[0] = getc(dish);
        if (isalpha (lexeme[0]) ) {
                for(i = 1; isalnum (lexeme[i] = getc(dish)); i < MAXID_SIZE ? i++ : i);
                ungetc (lexeme[i], dish);
		lexeme[i] = 0;
		if(i = iskeyword(lexeme)) return i;
                return ID;
        }
        ungetc (lexeme[0], dish);
        return 0;
}

int is_decimal(FILE *dish)
{
        int lexeme[0] = getc(dish);
        if (isdigit (lexeme[0]) ) {
                if (lexeme[0] == '0') {
                        return DEC;
                }
                // [0-9]*
                while ( isdigit (lexeme[0] = getc(dish)) );
                ungetc (lexeme[0], dish);
                return DEC;
        }
        ungetc (lexeme[0], dish);
        return 0;
}

int is_octal(FILE *dish)
{
        int octpref = getc(dish);
        if (octpref == '0') {
                int lexeme[0] = getc(dish);
                if ( lexeme[0] >= '0' && lexeme[0] <= '7') {
                        while ( (lexeme[0] = getc(dish)) >= '0' && lexeme[0] <= '7');
                        ungetc (lexeme[0], dish);
                        return OCTAL;
                } else {
                        ungetc (lexeme[0], dish);
                        ungetc (octpref, dish);
                        return 0;
                }
        }
        ungetc (octpref, dish);
        return 0;
}

int gettoken (FILE *tokenstream)
{
        int token;

        skipspaces (tokenstream);

        if ( token = is_identifier(tokenstream) ) {
                return ID;
        }

        if ( token = is_decimal (tokenstream) ) {
                return DEC;
        }

        token = getc (tokenstream);

        return token;
}
