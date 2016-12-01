#ifndef TABTOKEN_H
#define TABTOKEN_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* definições */
enum qualifier{
	RESERVED_WORD=500, /* qualifica como palavra reservada */
	ID_CON,		/* qualifica como constante, numérica ou literal */
	ID_TIP,		/* qualifica como tipo, do sistema ou definido pelo usuário */
	ID_VAR,		/* qualifica como uma variável */
	ID_PRO,		/* qualifica como um procedimento */
	ID_FUN,		/* qualifica como uma função */
   ID_LABEL,	/* qualifica como um rótulo */
	CONSTANT,	/* indica que o lexema é uma constante numérica*/
   LITERAL,		/* indica que o lexema é um string */
   LITERAL_CONSTANT, /* indica que é uma string já associada a uma variável */
	UNDEFINED	/* não qualificado */
};

enum tokens{
	PROGRAM=1000, BEGIN, END, LABEL, CONST, TYPE, VAR, PROCEDURE, FUNCTION,
	IF, THEN, ELSE, CASE, OF, WHILE, DO, REPEAT, UNTIL, FOR, TO, GOTO,
	ARRAY, ARQUIVO, BOOLEAN, INTEGER, CHAR, REAL, STRING, NOT, OR, DIV,
    MOD, COUNT_RESERVED_WORDS,
	IDENTIFICADOR, NUMBER, LITERAL_CHAR, LITERAL_TEXT,
	ASSIGN, PTO_PTO, LE, GE
};

enum types{BOOL=2000, INT, CHARACTER, FLOAT, TEXT, NOTYPE};

typedef enum boolean{FALSE, TRUE} boolean_t;

typedef enum qualifier qualifier_t;
typedef enum types type_t;
typedef struct tupla{
  char   lexema[50];	/* lexema lido da entrada */
  int    token;	  	/* token para o lexema */
  type_t type;		  	/* identifica o tipo da variável entre integer ou double */
  char	*offset;		/* salva o endereço de acesso da variável */
  qualifier_t qual;	/* qualifica um identificador */
} tupla_t;

/* variáveis */
extern tupla_t *currentp;
extern int lookahead;
extern char lexema[512];

/* protótipo das funções */
void inittab(void);
tupla_t *gettupla(int idx);
boolean_t settupla(int idx, tupla_t tp);
int getlast(void);				/* retorna o índice da última tupla 			*/
int retrieve(const char *s);	/* encontra e retorna o token do lexema 		*/
int findindex(const char *s);	/* encontra o índice de um lexema na tabela  */
int insert(const char *s, int token, const char *offset, qualifier_t qual, type_t type);
void printtupla(int idx);		/* impreme a tupla idx da tabela de símbolos */
void printtab(void);				/* imprime o conteúdo da tabela de símbolos  */
void trimtab(void);				/* ajusta a capacidade da tabela de símbolos
							 			 * para a quantidade de elementos contidos
                               * na tabela */
#endif
