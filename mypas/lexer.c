/**@<lexer.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <tokens.h>
#include <keywords.h>

#define MAXID_SIZE	32
char lexeme[MAXID_SIZE+1];

void skipspaces (FILE *tape)
{
    
	lexeme[0] = getc(tape);
    while ( lexeme[0] != '\n' && isspace (lexeme[0]) ) {
        lexeme[0] = getc(tape);
	}  // enquanto não atingir o fim de arquivo

        ungetc ( lexeme[0], tape );
}

//hoje
int isassign (FILE * tape)
{
	if ((lexeme[0] = getc(tape)) == ':') {
		if ((lexeme[1] = getc(tape)) == '=') {
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	return 0;
}
//-hoje

int isidentifier(FILE *tape)
{
	int token;

	if (isalpha (lexeme[0] = getc(tape)) ) {
                for(token = 1; isalnum (lexeme[token] = getc(tape)); token < MAXID_SIZE ? token++ : token);
                ungetc (lexeme[token], tape);
		lexeme[token] = 0;
		if(token = iskeyword(lexeme)) return token;
                return ID;
        }
        ungetc (lexeme[0], tape);
        return 0;
}
//hoje
int isint(FILE *tape)
{
        if (isdigit (lexeme[0] = getc(tape)) ) {
                if (lexeme[0] == '0') {
                        return INT;
                }
                // [0-9]*
                while ( isdigit (lexeme[0] = getc(tape)) );
                ungetc (lexeme[0], tape);
                return INT;
        }
        ungetc (lexeme[0], tape);
        return 0;
}
//-hoje

/*int isoctal(FILE *tape)
{
        int octpref = getc(tape);
        if (octpref == '0') {
                lexeme[0] = getc(tape);
                if ( lexeme[0] >= '0' && lexeme[0] <= '7') {
                        while ( (lexeme[0] = getc(tape)) >= '0' && lexeme[0] <= '7');
                        ungetc (lexeme[0], tape);
                        return OCTAL;
                } else {
                        ungetc (lexeme[0], tape);
                        ungetc (octpref, tape);
                        return 0;
                }
        }
        ungetc (octpref, tape);
        return 0;
}*/

int gettoken (FILE *sourcecode)
{
        int token;

        skipspaces (sourcecode);

        if ( token = isidentifier(sourcecode) ) {
                return ID;
        }

        if ( token = isint (sourcecode) ) {
                return INT;
        }
	
	lexeme[1] = 0;

        lexeme[0] = token = getc (sourcecode);

        return token;
}
