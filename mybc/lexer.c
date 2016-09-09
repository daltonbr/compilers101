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
    lexeme[0] = getc(stream);
    if (isdigit(lexeme[0]))
    {
        // at this point we have at least one decimal char valid
        if (lexeme[0] == '0')  // special case ZERO
        {
            lexeme[1] = 0;
            return DEC;
        }
        // [0-9]*
        int i = 0;
        do
        {
            i++;
			if (i > MAXSIZE_LEXEME)
			{
                char temp_char;	// used to read chars beyond MAXSIZE_LEXEME and feed back to the stream
				// reached max size, now stop buffering it...but keep reading
				lexeme[i-1] = 0; 	// terminating the string
				while (isdigit(lexeme[i] = getc(stream)));
				ungetc(temp_char, stream);
				printf("DEC - lexeme %s (cropped) \n", lexeme);
				return DEC;
			}
        }while (isdigit(lexeme[i] = getc(stream))); 
        ungetc(lexeme[i], stream);
        lexeme[i] = 0;
        return DEC;
    }
    ungetc(lexeme[0], stream);
    return 0;
}

int is_octal(FILE *stream)
{
	lexeme[0] = getc(stream);
	if (lexeme[0] == '0')  //octal prefix
	{
        // test for AT LEAST one Oct char after the prefix
		lexeme[1] = getc(stream);
		if (lexeme[1] >= '0' && lexeme[1] <= '7')
	    {
            int i = 2;
			while ( ((lexeme[i] = getc(stream)) >= '0') && lexeme[i] <= '7')
            {
                i++;
                if (i > MAXSIZE_LEXEME)
					{
						// reached max size, now stop buffering it...but keep reading
						char temp_char;	// used to read chars beyond MAXSIZE_LEXEME and feed back to the stream
						lexeme[i-1] = 0; 	// terminating the string
						while ( ((temp_char = getc(stream)) >= '0') && lexeme[i] <= '7');
						ungetc(temp_char, stream);
						printf("OCT - lexeme %s (cropped) \n", lexeme);
						return OCTAL;
					}
            }
			ungetc(lexeme[i], stream);
            lexeme[i] = 0;
            printf("OCT - lexeme %s \n", lexeme);
			return OCTAL;
		}
		ungetc(lexeme[1], stream);
	}
	ungetc(lexeme[0], stream);
	return 0;
}

int is_hexadecimal(FILE *stream)
{
    lexeme[0] = getc(stream);
    if (lexeme[0] == '0')
    {
        if ((toupper(lexeme[1] = getc(stream))) == 'X')
        {
            // test for AT LEAST one hex char after 'X'
            if (isxdigit(lexeme[2] = getc(stream)))
            {
                int i = 3;
                while (isxdigit(lexeme[i] = getc(stream)))
                {
                    i++;
                    if (i > MAXSIZE_LEXEME)
                    {
                        // reached max size, now stop lexemeing it...but keep reading
                        char temp_char; // used to read chars beyond MAXSIZE_LEXEME and feed back to the stream
                        lexeme[i-1] = 0;  // terminating the string
                        while (isxdigit(temp_char = getc(stream)));
                        ungetc(temp_char, stream);
                        printf("HEX - lexeme %s (cropped) \n", lexeme);
                        return HEXADEC;
                    }
                }
                ungetc(lexeme[i], stream);
                lexeme[i] = 0;
                printf("HEX - lexeme %s \n", lexeme);
                return HEXADEC;
            }
            ungetc(lexeme[2], stream);
        }
        ungetc(lexeme[1], stream);
    }
    ungetc(lexeme[0], stream);
    lexeme[0] = 0;
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
        if (( token = is_hexadecimal(stream)) ) {
                return HEXADEC;
        }
        if (( token = is_octal(stream)) ) {
                return OCTAL;
        }
        if (( token = is_decimal(stream)) ) {
                return DEC;
        }
        token = getc (stream);
        return token;
}
