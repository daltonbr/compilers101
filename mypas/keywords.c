/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>


char *keywords[] = {
        "begin",
        "if",
        "then",
        "elif",
        "else",
        "endif",
        "while",
        "do",
        "repeat",
        "until",
        "procedure",
        "function",
        "var",
        "boolean",
        "integer",
        "real",
        "double",
        "div",
        "mod",
        "and",
        "or",
        "not",
        "true",
        "false",
        "end"
};

int iskeyword(char const *identifier)
{
	int token;
	for(token = IF; token <= DONE; token++) {
		if( strcmp(keywords[token - IF], identifier) == 0 ) return token;			// IF é o offset
	}

	return 0;
}
