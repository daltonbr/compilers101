#include <stdlib.h>
#include <stdio.h>

#include "Lexer.h"
#include "Parser.h"
#include "ErrorHandler.h"
FILE           *tape;

int main(int argc, char **argv)
{
	if (argc > 1) {
		tape = fopen(argv[1], "r");
		if (tape == NULL) {
			fprintf(stderr, "File '%s' not found... \n", argv[1]);
			exit(-2);
		}
	} else {
		tape = stdin;
		//tape = fopen("arquivo.pas", "r+");
	}

	lookahead = get_token(tape);
	program();
	if (!semantic_error) {
		printf("Successfully compiled\n");
	}
/*
	int tok;

	while((tok = get_token(tape)) != EOF) {
		printf("TOKEN: %d\t", tok);
		printf("LEXEME: '%s'\n", lexeme);
	}
*/
	return 0;
}
