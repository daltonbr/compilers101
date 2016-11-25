
enum {
	UNKNOWN_ERROR = -0x100,
	SYMBOL_OVERLAP,
	SYMBOL_OVERFLOW,
	SYMBOL_NOT_FOUND,
	INVALID_OPERAND,
	INVALID_TYPE,
};

void            fatal_error(int, char*);

extern int semantic_error;
