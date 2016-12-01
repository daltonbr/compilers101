/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>


char *keywords[] = {
        "begin",//524288
        "if",//524289
        "then",//524290
        "elif",//524291
        "else",//524292
        "endif",//524293
        "while",//524294
        "do",//524295
        "repeat",//524296
        "until",//524297
        "procedure",//524298
        "function",//524299
        "var",//524300
        "boolean",//524301
        "integer",//524302
        "real",//524303
        "double",//524304
        "div",//524305
        "mod",//524306
        "and",//524307
        "or",//524308
        "not",//524309
        "true",//524310
        "false",//524311
        "end"//524312
};

int iskeyword(char const *identifier)
{
	int token;
	for(token = BEGIN; token <= END; token++) {
		if( strcmp(keywords[token - BEGIN], identifier) == 0 ) return token;			// IF é o offset
	}

	return 0;
}
