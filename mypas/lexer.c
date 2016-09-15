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
	lexeme[0] = getc(dish);
        while ( lexeme[0] != '\n' && isspace (lexeme[0]) ) {
		lexeme[0] = getc(dish);
	}  // enquanto não atingir o fim de arquivo

        ungetc ( lexeme[0], dish );
}

int isidentifier(FILE *dish)
{
	int token;

	if (isalpha (lexeme[0] = getc(dish)) ) {
                for(token = 1; isalnum (lexeme[token] = getc(dish)); token < MAXID_SIZE ? token++ : token);
                ungetc (lexeme[token], dish);
		lexeme[token] = 0;
		if(token = iskeyword(lexeme)) return token;
                return ID;
        }
        ungetc (lexeme[0], dish);
        return 0;
}

int isdecimal(FILE *dish)
{
        if (isdigit (lexeme[0] = getc(dish)) ) {
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

int isoctal(FILE *dish)
{
        int octpref = getc(dish);
        if (octpref == '0') {
                lexeme[0] = getc(dish);
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

int gettoken (FILE *sourcecode)
{
        int token;

        skipspaces (sourcecode);

        if ( token = isidentifier(sourcecode) ) {
                return ID;
        }

        if ( token = isdecimal (sourcecode) ) {
                return DEC;
        }
	
	lexeme[1] = 0;

        lexeme[0] = token = getc (sourcecode);

        return token;
}
