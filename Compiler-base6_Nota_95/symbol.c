#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "lexan.h"

#define EPSILON ;
#define SUCCESS 100

int lexentry;

/*
 * t_bottom aponta para a proxima posicao da tabela de simbolos
*/
int t_bottom = 0;

/*
 * endstream aponta para a proxima posicao livre de lexstream
*/
int endstream = 0;

/*
 * sentinela aponta para o inicio da declaracao das variaveis do programa
 * apos a declaracao das palavras reservadas)
 */
int sentinela;

/*
 * array onde serao salvos os "identificadores"
*/
char lexstream[MAX_STR];

/*
 * table sera a tabela de simbolos
*/
symbol_table table[MAX_T];

/*
 * level indica o atual nivel lexico do programa Pascal
 */
int level;
int reg_ati;

/*
 * funcao que busca um simbolo na tabela de simbolos. Se encontra-lo retorna
 * o valor da entrada na tabela, caso contrario retorna -1
*/

int busca(char *simbolo){
	int cont;

	/*
	 * verifica se esta entre as palavras reservadas
	 */
	for(cont = 0; cont < sentinela; cont++)
		if(strcmp(table[cont].pos,simbolo)==0)
			return cont;   
	/*
	 * verifica entre as variaveis
	 */
			
	for(cont = t_bottom-1; cont >= sentinela; cont--)
		if(strcmp(table[cont].pos,simbolo) == 0)	
			if(table[cont].nivel == level)		 /* verifica se esta tentando declarar a mesma */
			return cont;				 /* variavel duas vezes na mesmo nivel*/	
	return -1;						
}
/*
 * Insere um novo simbolo na tabela de simbolos
*/
void insercao(char *simbolo,int tok,int set, int type){
    char *p = lexstream + endstream;
    	
    /*
     * verifica se a tabela ja esta cheia
     */
    if (endstream > MAX_STR)
	printf("\n A tabela ja esta cheia");
    else if (t_bottom > MAX_T)
	printf("\n A tabela esta cheia ");
    else {
	strcpy(p, simbolo);	
	table[t_bottom].pos = p;
	table[t_bottom].valor = 0;
	table[t_bottom].token = tok;
	table[t_bottom].offset = set;
	table[t_bottom].type = type;
	table[t_bottom].nivel = 0;
	t_bottom++;

	endstream += strlen(simbolo) + 1;	
    }
}

/*
 * Insere as palavras reservadas na tabela de simbolos
*/
void reservadas(void){
	
	insercao("program",PROGRAM,0,NA);	
	insercao("begin",BEGIN,0,NA);        
	insercao("end",END,0,NA);            
	insercao("procedure",PROCEDURE,0,NA);
	insercao("function",FUNCTION,0,NA);  
	insercao("var",VAR,0,NA);            
	insercao("const",CONST,0,NA);        
	insercao("type",TYPE,0,NA);          
	insercao("if",IF,0,NA);              
	insercao("then",THEN,0,NA);          
	insercao("else",ELSE,0,NA);          
	insercao("case",CASE,0,NA);          
	insercao("of",OF,0,NA);              
	insercao("while",WHILE,0,NA);        
	insercao("do",DO,0,NA);              
	insercao("repeat",REPEAT,0,NA);      
	insercao("until",UNTIL,0,NA);        
	insercao("for",FOR,0,NA);            
	insercao("to",TO,0,NA);              
	insercao("mod",MOD,0,NA);            
	insercao("div",DIV,0,NA);            
	insercao("and",AND,0,NA);            
	insercao("or",OR,0,NA);
	insercao("not",NOT,0,NA);
	insercao("char",CHAR,0,NA);
	insercao("integer",INTEGER,0,NA);
	insercao("real",REAL,0,NA);
	insercao("boolean",BOOLEAN,0,NA);
	insercao("abs",ABS,0,NA);
	insercao("read",READ,0,NA);
	insercao("write",WRITE,0,NA);

	sentinela=t_bottom;
}
