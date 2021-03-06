/*
* Compilers - Class Exercise - august 2, 2016
* Professor: Eraldo Marinho
* Author: Dalton Lima
* version: 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  //C99 requires to use toupper()

#define MAXSIZE_LEXEME  8 // Lexeme Max Buffer Size
#define DEC 	1
#define OCTAL 	2
#define HEXADEC	3
#define FIXP  	4 //this is a floating point WITHOUT the expoent
#define ID			8192

int scanid(FILE *tape);
int scandec(FILE *tape);
int scanoct(FILE *tape);
int scanhex(FILE *tape);
int scanfixp(FILE *tape);

char buffer[MAXSIZE_LEXEME+1];  // TODO: Must be local to the methods?

int scanid(FILE *tape)
{
	char temp_char;	// used to read chars beyond MAXSIZE_LEXEME and feed back to the stream
	buffer[0] = getc(tape);
	if (isalpha(buffer[0]))
	{
		int i = 1;
		while (isalnum(buffer[i] = getc(tape)))
		{
			i++;
			if (i >= MAXSIZE_LEXEME)
			{
				// reached max size, now stop buffering it...but keep reading
				buffer[i] = 0; 	// terminating the string
				while (isalnum(temp_char = getc(tape)));
				ungetc(temp_char, tape);
				printf("ID - lexeme %s (cropped) \n", buffer);
				return ID;
			}
		}
		ungetc(buffer[i], tape);
		buffer[i] = 0;
		printf("ID - lexeme %s \n", buffer);
		return ID;
	}
	ungetc(buffer[0], tape);
	buffer[0] = 0;  // Really necessary?
	return 0;
}

int scanhex(FILE *tape)
{
	buffer[0] = getc(tape);
	if (buffer[0] == '0')
	{
		if ((toupper(buffer[1] = getc(tape))) == 'X')
		{
			// test for AT LEAST one hex char after 'X'
			if (isxdigit(buffer[2] = getc(tape)))
			{
				int i = 3;
				while (isxdigit(buffer[i] = getc(tape)))
				{
				i++;
					if (i >= MAXSIZE_LEXEME)
					{
						// reached max size, now stop buffering it...but keep reading
						char temp_char;	// used to read chars beyond MAXSIZE_LEXEME and feed back to the stream
						buffer[i] = 0; 	// terminating the string
						while (isxdigit(temp_char = getc(tape)));
						ungetc(temp_char, tape);
						printf("HEX - lexeme %s (cropped) \n", buffer);
						return HEXADEC;
					}
				}
				ungetc(buffer[i], tape);
				buffer[i] = 0;
				printf("HEX - lexeme %s \n", buffer);
				return HEXADEC;
			}
			ungetc(buffer[2], tape);
		}
		ungetc(buffer[1], tape);
	}
	ungetc(buffer[0], tape);
	buffer[0] = 0;
	return 0;
}

int scandec(FILE *tape)
{
	int buffer[2];
	buffer[0] = getc(tape);
	if (isdigit(buffer[0]))
	{
		if (buffer[0] == '0')
		{
			return DEC;
		}
		// [0-9]*
		while (isdigit(buffer[1] = getc(tape)));
		ungetc (buffer[1], tape);
		return DEC;
	}
	ungetc (buffer[0], tape);
	return 0;
}

int scanoct(FILE *tape)
{
	int buffer[3];
	buffer[0] = getc(tape);
	if (buffer[0] == '0')
	{
		buffer[1] = getc(tape);
		if (buffer[1] >= '0' && buffer[1] <= '7')
	  {
			while ( ((buffer[2] = getc(tape)) >= '0') && buffer[2] <= '7');
			ungetc(buffer[2], tape);
			return OCTAL;
		}
		ungetc(buffer[1], tape);
	}
	ungetc(buffer[0], tape);
	return 0;
}

int scanfixp(FILE *tape)
{
	int buffer[3];
	if (scandec(tape))
	{
		buffer[0] = getc(tape);
		if (buffer[0] == '.')
		{
			while (isdigit(buffer[1] = getc(tape)));
			ungetc (buffer[1], tape);
			return FIXP;
		}
		ungetc(buffer[0], tape);
	}
	// starting with '.'
	else if ((buffer[0] = getc(tape)) == '.')
	{
		// must have AT LEAST one decimal char
		if (isdigit(buffer[1] = getc(tape)))
		{
			while (isdigit(buffer[2] = getc(tape)));
			ungetc (buffer[2], tape);
			return FIXP;
		}
		ungetc(buffer[1], tape);
		ungetc(buffer[0], tape);
	}
	rewind(tape);
	return 0;
}

int main (int argc, char *argv[], char *envp[])
{
	FILE *buffer;

	if (argc == 1) {
		buffer = stdin;
	} else {
		buffer = fopen (argv[1], "r");
		if (buffer == NULL) {
			fprintf (stderr, "%s: cannot open %s... exiting\n",
				argv[0], argv[1]);
			exit (-1);
		}
	}

	if (scanid(buffer)) {
		printf ("identifier\n");
	} else if (scanhex(buffer)) {
		printf ("hexadecimal\n");
	} else if (scanoct(buffer)) {
		printf ("octal\n");
	} else if (scanfixp(buffer)) {
		printf ("fixed point\n");
	} else if (scandec(buffer)) {
		printf ("decimal\n");
	} else {
		printf ("parsed string is not a decimal, octal, hexadecimal nor id\n");
	}
	exit (0);

}
