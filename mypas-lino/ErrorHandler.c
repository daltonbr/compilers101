#include <stdio.h>
#include <stdlib.h>
#include "ErrorHandler.h"
#include "Lexer.h"

int semantic_error;

char *error_list[] = {
	"Unknown error while compiling",
    "The specified symbol already exists",
	"The symbol table is full",
	"Symbol not found on symbol table",
	"Invalid operand",
	"Invalid type"
};

char* get_error(int error_code) {
	return error_list[error_code - UNKNOWN_ERROR];
}

void fatal_error(int error_code, char* ex_info)
{
	semantic_error = 1;
    fprintf(stderr, "FATAL ERROR [%i] [%s] on line %i", error_code, get_error(error_code), line_count);
    if(ex_info) {
    	fprintf(stderr, " [%s]", ex_info);
    }
    fprintf(stderr, "\n");
}
