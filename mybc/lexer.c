/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

void skipspaces (FILE *stream)
{
        int temp_char;
        while ( isspace ( temp_char = getc (stream) ) );
        ungetc ( temp_char, stream );
}

char lexeme[MAXID_SIZE+1];//@ lexer.c

int is_identifier(FILE *stream)
{
        int i = 0;
        lexeme[i] = getc(stream);
        if (isalpha (lexeme[i]) ) {
                for (i++; isalnum (lexeme[i] = getc(stream)); i++);
                ungetc (lexeme[i], stream);
                lexeme[i] = 0;
                return ID;
        }
        ungetc (lexeme[i], stream);
        return 0;
}

int is_decimal(FILE *stream)
{
        int i;
        if (isdigit (lexeme[0] = getc(stream)) ) {
                if (lexeme[0] == '0') {
			lexeme[1] = 0;
                        return DEC;
                }
                // [0-9]*
                for (i = 1; isdigit (lexeme[i] = getc(stream)); i++);
                ungetc (lexeme[i], stream);
		lexeme[i] = 0;		
                return DEC;
        }
        ungetc (lexeme[0], stream);
        return 0;
}

int is_octal(FILE *stream)
{
        int octpref = getc(stream);
        if (octpref == '0') {
                int temp_char = getc(stream);
                if ( temp_char >= '0' && temp_char <= '7') {
                        while ( (temp_char = getc(stream)) >= '0' && temp_char <= '7');
                        ungetc (temp_char, stream);
                        return OCTAL;
                } else {
                        ungetc (temp_char, stream);
                        ungetc (octpref, stream);
                        return 0;
                }
        }
        ungetc (octpref, stream);
        return 0;
}

int is_separator(FILE *stream) 
{
	int seppref = getc(stream);
	if(seppref == ';')
		return CMDSEP;

	ungetc(seppref, stream);
	return 0;
}

int gettoken (FILE *stream)
{
        int token;
        skipspaces (stream);
        if (( token = is_identifier(stream)) ) {
                return ID;
        }
        if (( token = is_decimal(stream)) ) {
                return DEC;
        }
        token = getc (stream);
        return token;
}
