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

int isdecimal (FILE *tape) {
	int i = 0;

	if (isdigit (lexeme[i] = getc(tape) ) ) {
		if (lexeme[i] == '0') {
			++i;
			lexeme[i] = getc(tape);
			return i;
		}
		// [0-9]*
		for(++i; isdigit( lexeme[i] = getc(tape) ); ++i) {
			if(i >= MAXID_SIZE) i = MAXID_SIZE;
		}
		return i;
	}
	return 0;
}


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

        if( (token = isidentifier(sourcecode)) ) {
		    return token;
	    }

	    if( (token = isassign(sourcecode)) ) {
		    return token;
	    }

	    if( (token = isfloat (sourcecode)) ) {
		    return token;
	    }
	
	    lexeme[1] = 0;
        lexeme[0] = token = getc (sourcecode);
        return token;
}
