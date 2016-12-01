/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>

char           *keyword[] = {
    "begin",
    "if",
    "then",
    "else",
    "while",
    "do",
    "repeat",
    "until",
    "div",
    "mod",
    "or",
    "and",
    "not",
    "true",
    "false",
    "program",
    "var",
    "integer",
    "real",
    "double",
    "char",
    "text",
    "boolean",
    "array",
    "of",
    "procedure",
    "function",
    "end"
};

int
iskeyword(char const *identifier)
{
    int             token;
    for (token = BEGIN; token <= END; token++) {
        if (strcmp(keyword[token - BEGIN], identifier) == 0)
            return token;
    }
    return 0;
}
