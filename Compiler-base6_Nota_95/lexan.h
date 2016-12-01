enum{
	ID = 0x8000,		/*(8000) define um identificador*/
	NUM_I, 			/*(8001) define um numero inteiro*/
	OP_ASSIG,		/*(8002) define o operador de atribuicao*/
	OP_ADD,			/*(8003) define operador adicao*/
	OP_SUB,			/*(8004) define operador subtracao*/
	OP_MUL,			/*(8005) define operador multiplicacao*/
	OP_DIV,			/*(8006) define operador divisao*/
	OP_EQ,			/*(8007) define o operador de igualdade*/
	OP_DIF,			/*(8008) define o operador <>(difereca)*/
	OP_LE,			/*(8009) define o operador <=*/
	OP_LT,			/*(800a) define o operador <*/
	OP_ME,			/*(800b) define o operador >=*/
	OP_MT,			/*(800c) define o operador >*/
	PROGRAM,                /*(800d)*/
	BEGIN,                  /*(800e)*/
	END,                    /*(800f)*/
	PROCEDURE,              /*(8010)*/
	FUNCTION,               /*(8011)*/
	VAR,                    /*(8012)*/
	CONST,			/*(8013)*/
	TYPE,			/*(8014)*/
	IF,                     /*(8015)*/
	THEN,                   /*(8016)*/
	ELSE,                   /*(8017)*/
	CASE,                   /*(8018)*/
	OF,                     /*(8019)*/
	WHILE,                  /*(801a)*/
	DO,                     /*(801b)*/
	REPEAT,                 /*(801c)*/
	UNTIL,                  /*(801d)*/
	FOR,                    /*(801e)*/
	TO,                     /*(801f)*/
	MOD,                    /*(8020)*/
	DIV,			/*(8021)*/
	AND,                    /*(8022)*/
	OR,                     /*(8023)*/
	NOT,                    /*(8024)*/
	PONTO,			/*(8025)*/
	CHAR,			/*(8026)*/
	BOOLEAN,		/*(8027)*/
	INTEGER,		/*(8028)*/
	REAL,			/*(8029)*/
	ABS,			/*(802a)*/
	READ,			/*(802b)*/
	WRITE,			/*(802c)*/
	CADEIA,			/*(802d)*/
	PT,			/*(802e)*/
	NUM_R			/*(802f)*/
};

enum{
	PROG = 0,		/*(0)*/
	BT,			/*(1)*/
	SHRT,			/*(2)*/
	INT,			/*(3)*/
	RL,			/*(4)*/
	DBL,			/*(5)*/
	CH,			/*(6)*/
	TXT,			/*(7)*/
	BLN,			/*(8)*/
	NA,			/*(9)*/
	err_tipo		/*(10)*/
};

extern FILE *buffer;

/*
 * lexeme sera o vetor onde serao salvos os identificadores, os
 * operadores logicos(and,or,etc), os operadores relacionais (<,<>,etc)
*/
extern char lexeme[256];

/*
 * lexnum guardara o valor de um numero lido de buffer
*/
extern float lexnum;

extern int num1;
/*
 * linha guarda a linha do programa que esta sendo compilada
*/
extern int linha;

typedef int token_t;
extern token_t lookahead;
token_t gettoken(void);
token_t  match(token_t token);
