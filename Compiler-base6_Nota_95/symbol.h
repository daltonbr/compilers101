#ifndef SYMBOL_H
#define SYMBOL_H

struct mytable{
	char *pos;		/*ponteiro para o vetor dos lexemas*/
	double valor;		/*local onde sera armazenado o valor atual da variavel*/
	int token;		/*token do simbolo*/
	int offset;		/*offset do simbolo*/
	int type;		/*tipo do simbolo*/
	int nivel;		/*nivel lexico do simbolo*/
};

/*
 * define a quantidade maxima de tokens que poderao existir na tabela
*/
#define MAX_T 256

/*
 * define o tamanho maximo do vetor que ira armazenar os lexemas
*/
#define MAX_STR 10240

typedef struct mytable symbol_table;

extern int lexentry;

/*
 * t_bottom aponta para a proxima posicao da tabela de simbolos
*/
extern int t_bottom;

/*
 * endstream aponta para a proxima posicao livre de lexstream
*/
extern int endstream;

/*
 * array onde serao salvos os "identificadores"
*/
extern char lexstream[MAX_STR];

/*
 * table sera a tabela de simbolos
*/
extern symbol_table table[MAX_T];

/*
 * funcao que busca um simbolo na tabela de simbolos
*/
int busca(char *simbolo);
   
/*
 * Insere um novo simbolo na tabela de simbolos
*/
void insercao(char *simbolo, int tok,int set,int type);

/*
 * insere as palavras reservadas na tabela de simbolos antes de iniciar a analise sintatica
*/
void reservadas(void);

#endif
