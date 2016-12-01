#ifndef TABTOKEN_H
#define TABTOKEN_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* defini��es */
enum qualifier{
	RESERVED_WORD=500, /* qualifica como palavra reservada */
	ID_CON,		/* qualifica como constante, num�rica ou literal */
	ID_TIP,		/* qualifica como tipo, do sistema ou definido pelo usu�rio */
	ID_VAR,		/* qualifica como uma vari�vel */
	ID_PRO,		/* qualifica como um procedimento */
	ID_FUN,		/* qualifica como uma fun��o */
   ID_LABEL,	/* qualifica como um r�tulo */
	CONSTANT,	/* indica que o lexema � uma constante num�rica*/
   LITERAL,		/* indica que o lexema � um string */
   LITERAL_CONSTANT, /* indica que � uma string j� associada a uma vari�vel */
	UNDEFINED	/* n�o qualificado */
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
  type_t type;		  	/* identifica o tipo da vari�vel entre integer ou double */
  char	*offset;		/* salva o endere�o de acesso da vari�vel */
  qualifier_t qual;	/* qualifica um identificador */
} tupla_t;

/* vari�veis */
extern tupla_t *currentp;
extern int lookahead;
extern char lexema[512];

/* prot�tipo das fun��es */
void inittab(void);
tupla_t *gettupla(int idx);
boolean_t settupla(int idx, tupla_t tp);
int getlast(void);				/* retorna o �ndice da �ltima tupla 			*/
int retrieve(const char *s);	/* encontra e retorna o token do lexema 		*/
int findindex(const char *s);	/* encontra o �ndice de um lexema na tabela  */
int insert(const char *s, int token, const char *offset, qualifier_t qual, type_t type);
void printtupla(int idx);		/* impreme a tupla idx da tabela de s�mbolos */
void printtab(void);				/* imprime o conte�do da tabela de s�mbolos  */
void trimtab(void);				/* ajusta a capacidade da tabela de s�mbolos
							 			 * para a quantidade de elementos contidos
                               * na tabela */
#endif
