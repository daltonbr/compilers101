/**@<lexer.c>::**/

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

char lexeme[MAXSIZE_LEXEME+1];//@ lexer.c

int is_identifier(FILE *stream)
{
	char temp_char;	// used to read chars beyond MAXSIZE_SIZE and feed back to the stream
	lexeme[0] = getc(stream);
	if (isalpha(lexeme[0]))
	{
		int i = 1;
		while (isalnum(lexeme[i] = getc(stream)))
		{
			i++;
			if (i > MAXSIZE_LEXEME)
			{
				// reached max size, now stop lexemeing it...but keep reading
				lexeme[i-1] = 0; 	// terminating the string
				while (isalnum(temp_char = getc(stream)));
				ungetc(temp_char, stream);
				printf("Debug: ID - lexeme %s (cropped) \n", lexeme);
				return ID;
			}
		}
		ungetc(lexeme[i], stream);
		lexeme[i] = 0;
		printf("Debug: ID - lexeme %s \n", lexeme);
		return ID;
	}
	ungetc(lexeme[0], stream);
	lexeme[0] = 0;
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
