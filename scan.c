#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  //C99 requires to use toupper()

#define DEC 		1
#define OCTAL 	2
#define HEXADEC	3
#define PTF		 	4 //this is a floating point WITHOUT the expoent
#define ID			8192

int scanid(FILE *tape);
int scandec(FILE *tape);
int scanoct(FILE *tape);
int scanhex(FILE *tape);
//int scanptf(FILE *tape);

int scanid(FILE *tape)
{
	int head = getc(tape);
	if ((toupper(head)) >= 'A' && toupper(head) <= 'Z')
	{
		while ( ((toupper(head = getc(tape))) >= 'A' && toupper(head) <= 'Z')
			|| (head >= '0' && head <= '9'));
		ungetc(head, tape);
		return ID;
	}
	ungetc(head, tape);
	return 0;
}

int scanhex(FILE *tape)
{
	int buffer[4];
	buffer[0] = getc(tape);
	if (buffer[0] == '0')
	{
		if ((toupper(buffer[1] = getc(tape))) == 'X')
		{
			// test for AT LEAST one hex char after 'X'
			// special case? 0x0 ?
			if ( ((toupper(buffer[2] = getc(tape))) >= 'A' && (toupper(buffer[2])) <= 'F')
				|| (buffer[2] >= '0' && buffer[2] <= '9') )
			{
				while ( ((toupper(buffer[3] = getc(tape))) >= 'A' && (toupper(buffer[3])) <= 'F')
					|| (buffer[3] >= '0' && buffer[3] <= '9') );
				ungetc(buffer[3], tape);
				return HEXADEC;
			}
			ungetc(buffer[2], tape);
		}
		ungetc(buffer[1], tape);
	}
	ungetc(buffer[0], tape);
	return 0;
}

int scandec(FILE *tape)
{
	int head = getc(tape);
	if (head >= '0' && head <= '9') {
		if (head == '0') {
			return DEC;
		}
		// [0-9]*
		while ( (head = getc(tape)) >= '0' && head <= '9');
		ungetc (head, tape);
		return DEC;
	}
	ungetc (head, tape);
	return 0;
}

int scanoct(FILE *tape)
{
	int octPrefix = getc(tape);
	if (octPrefix == '0')
	{
		int head = getc(tape);
		// special case: octal zero
		if (head == '0')
		{
			ungetc(head, tape);
			return OCTAL;
		}
		if (head >= '1' && head <= '7')
	  {
			while ( ((head = getc(tape)) >= '0') && head <= '7');
			ungetc(head, tape);
			return OCTAL;
		}
		ungetc(head, tape);
	}
	ungetc(octPrefix, tape);
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
	} else if (scandec (buffer)) {
		printf ("decimal\n");
	} else {
		printf ("parsed string is not a decimal, octal, hexadecimal nor id\n");
	}
	exit (0);

}
