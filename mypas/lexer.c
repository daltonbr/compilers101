/**@<lexer.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <lexer.h>
#include <tokens.h>
#include <keywords.h>

#define MAXID_SIZE	32
char lexeme[MAXID_SIZE+1];
int line_count = 1;

int skipspaces (FILE *tape)
{
	lexeme[0] = getc(tape);
    while ( isspace (lexeme[0]) ) {
        lexeme[0] = getc(tape);
	}  // enquanto não atingir o fim de arquivo

    ungetc ( lexeme[0], tape );

    return 0;
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

	if (isalpha ((lexeme[0] = getc(tape))) ) {
                for(token = 1; isalnum ((lexeme[token] = getc(tape))); token < MAXID_SIZE ? token++ : token);
                ungetc (lexeme[token], tape);
		lexeme[token] = 0;
		if((token = iskeyword(lexeme))) return token;
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

int isfloat(FILE *tape) {
	int i = 0;

	if( (i = isdecimal(tape)) ) {
		if( lexeme[i] == '.') {
			for (i++; isdigit (lexeme[i] = getc(tape)); i++) {
				if(i >= MAXID_SIZE) i = MAXID_SIZE;
			}
			isexp(tape, &i);
			ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
			return FLT;
		}
		else if(isexp(tape, &i)) {
			ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
			return FLT;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = '\0';
		return DEC;
	}
	else if(lexeme[i] == '.') {
		++i;
		lexeme[i] = getc(tape);
		if( isdigit(lexeme[i]) ) { 
			for (i++; isdigit (lexeme[i] = getc(tape)); i++) {
				if(i >= MAXID_SIZE) i = MAXID_SIZE;
			}
			isexp(tape, &i);
			ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
			return FLT;
		}
		ungetc(lexeme[i], tape);
		--i;
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = '\0';
	return 0;
}

int isexp (FILE *tape, int *count) {

	if( toupper( lexeme[(*count)] ) == 'E') {
		++(*count);
		if( (lexeme[(*count)] = getc(tape) )== '+' || lexeme[(*count)]== '-') {
				++(*count);
				if(isdigit( lexeme[(*count)]  = getc(tape) )) {
					for ( (*count)++; isdigit (lexeme[(*count)] = getc(tape)); (*count)++) {
						if( (*count) >= MAXID_SIZE) (*count) = MAXID_SIZE;
					}
					return 1;
				}
		}
		else if(isdigit(lexeme[(*count)])) {
			for ((*count)++; isdigit (lexeme[(*count)] = getc(tape)); (*count)++) {
				if((*count) >= MAXID_SIZE) (*count) = MAXID_SIZE;
			}
				return 1;
		}
		ungetc(lexeme[(*count)], tape);
		--(*count);
		return 0;
	}
	return 0;
}

int is_string(FILE *tape) {
    int i = 0;

    if ((lexeme[i] = getc(tape)) == '\"') {
        i++;
        if( isalnum(lexeme[i] = getc(tape)) && lexeme[i] != '\"' || lexeme[i] == '\n') {
            i++;
            while (isalnum(lexeme[i] = getc(tape)) && lexeme[i] != '\"' && lexeme[i] != '\n' || lexeme[i] == ' ') i++;
        }

        if(lexeme[i] == '\"') {
            return STR;
        } else {
            ungetc(lexeme[i], tape);
            while(i > 0) {
                i--;
                ungetc(lexeme[i], tape);
            }
        }
    } else {
        ungetc(lexeme[i], tape);
    }

    return 0;
}

int gettoken (FILE *sourcecode)
{
        int token;
        if(token = skipspaces (sourcecode)) {
            return token;
        }

        if( (token = isidentifier(sourcecode)) ) {
		    return token;
	    }

	    if( (token = isassign(sourcecode)) ) {
		    return token;
	    }

	    if( (token = isfloat (sourcecode)) ) {
		    return token;
	    }

        if( (token = is_string(sourcecode)) ) {
            return token;
        }

	    lexeme[1] = 0;
        lexeme[0] = token = getc (sourcecode);
        return token;
}
