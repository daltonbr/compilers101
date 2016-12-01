#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexan.h"
#include "symbol.h"
#include "parser.h"

/*
 * Definicao do simbolo vazio
*/
#define EPSILON ;
#define SUCCESS 100

token_t lookahead;
int err=0;

int lexentry;

/*
 * buffer  e o vetor onde estara o programa a ser compilado
*/
FILE *buffer;

/*
 * lexeme sera o vetor onde serao salvos os identificadores, os
 * operadores logicos(and,or,etc), os operadores relacionais (<,<>,etc)
*/
char lexeme[256];

/*
 * lexnum guardara o valor de um numero lido de buffer
*/
float lexnum=0;

int num1;
void err_handl(int tok, int look){
	printf("\n LINHA: %d, TOKEN: %x, LOOKAHEAD: %x antes de %s",linha,tok,look,lexeme);
	exit(2);
}

/*
 * Funcao que verifica se o token esperado e igual ao token lido
*/
token_t match(token_t token){
	if(token == lookahead){
		lookahead = gettoken();
		return SUCCESS;
	}
	else {
		err_handl(token,lookahead);
		return 0;
	}
}

/*
 * Funcao que busca um novo token em buffer, retornando o valor desse
 * token, se este pertencer a linguagem ou retorna o proprio simbolo
 * lido caso este nao esteja entre os tokens declarados
*/
token_t gettoken(void){
	int symbol,i=0,t=0,aux,d=0,r;		 
	div_t x;	
	/*
	 * ignora brancos
	*/
	do{
		symbol = getc(buffer);
		/*
		 * incrementa contador de linhas
		*/
		if (symbol == '\n') linha++;
	}while (isspace(symbol) && (symbol != EOF));

	/*
	 * retorna token para identificador ou palavra-reservada
	*/
	if(isalpha(symbol)){
		ungetc(symbol,buffer);
		fscanf(buffer,"%[A-Za-z_0-9]",lexeme);
		lexentry = busca(lexeme);
		if (lexentry == -1) {
			insercao(lexeme,ID,0,0);
			lexentry = t_bottom-1;
			return ID;
		}	
		else return table[lexentry].token;			
	}
	/*
	* retorna token para um numero inteiro, real ou da forma <num>.<num>E(+/-)<num>
	*/	
	else if(isdigit(symbol)){
		ungetc(symbol,buffer);
		fscanf(buffer," %d",&num1);		
		symbol=getc(buffer);
		if(symbol == '.'){
			/* 
			 * se simbolo for '.' devolve tudo o que ja leu para o buffer
			 * e pega o numero como ponto flutuante
			 */
			ungetc(symbol,buffer);
			r = num1;				
			while(r >= 10){
				x = div(r,10);
				r=x.quot;				
				d++;				
			}					
			fseek(buffer,-(d+1),SEEK_CUR);  
			fscanf(buffer," %f",&lexnum);			
			return NUM_R;
		}
		else { 
			/*symbol !='.'*/				
			ungetc(symbol,buffer);						
			return NUM_I;
		}	
	}	
	/*
	 * RETORNA OPERADORES, DELIMITADORES e PONTEIRO
	 * retorna token para '+'
	*/
	else if(symbol == '+' ){
		lexeme[0] = symbol;
		lexeme[1] = '\0';
		return OP_ADD;
	  }
	  /*
	   * retorna token para '-'
	  */
	  else if(symbol == '-'){
		  lexeme[0] = symbol;
		  lexeme[1] = '\0';
		  return OP_SUB;
	  }
	  /*
	   * retorna token para '*'
	  */
	  else if(symbol == '*') {
		  lexeme[0] = symbol;
		  lexeme[1] = '\0';
		  return OP_MUL;
	  }
	  /*
	   * retorna token para '/'
	  */
	  else if(symbol == '/') {
		  lexeme[0] = symbol;
		  lexeme[1] = '\0';
		  return OP_DIV;
	  }
	  /*
	   * retorna token para ':=' ou ':'
	  */
	  else if (symbol == ':'){
		    lexeme[0] = symbol;
		    symbol=getc(buffer);  
		    if(symbol == '=') {
			    lexeme[1] = symbol;
			    lexeme[2] = '\0';
			    return OP_ASSIG;
		    }	    
		    ungetc(symbol,buffer);
		    lexeme[1]='\0';
		    return PONTO;
		}
	  /*
	   * retorna token para '<>', ou '<=', ou '<'
	  */
	  else if(symbol == '<'){	
		 lexeme[0] = symbol; 
	         symbol = getc(buffer);
	         if(symbol == '>') {
			 lexeme[1] = symbol;
			 lexeme[2] = '\0';
			 return OP_DIF;	 
		 }	 
	         else if(symbol == '=') {
			 lexeme[1] = symbol;
			 lexeme[2] = '\0';
			 return OP_LE;
		 }	 
	         else {
	             ungetc(symbol,buffer);
		     lexeme[1] = '\0';
    	             return OP_LT;
	        }   
	 }
	 /*
	  * retorna token para '>' ou para '>='
	 */
	 else if(symbol == '>'){
		 lexeme[0] = symbol;
	         symbol = getc(buffer);
	         if(symbol == '=') {
			 lexeme[1] = symbol;
			 lexeme[2] = '\0';
			 return OP_ME;	
		 }	 
		 lexeme[1] = '\0';
	         ungetc(symbol,buffer);	
	         return OP_MT;			
	}
	/*
	 * retorna token para '='
	*/
	else if (symbol == '='){
		lexeme[0]=symbol;
		lexeme[1]='\0';
		return OP_EQ;
	}
	/*
	 * retorna token para '..' para a declaracao de arrays
	*/
	else if (symbol == '.'){
		lexeme[0] = symbol;		
		lexeme[1] = '\0';
		return PT;
	}
	/*
	 * retorna CADEIA, que indica uma cadeia de caracteres entre aspas simples
	*/
	else if (symbol == 39){
		aux = symbol;
		symbol = getc(buffer);
		if(isalpha(symbol)){
			ungetc(symbol,buffer);
			fscanf(buffer,"%[A-Za-z]",lexeme);
			t = strlen(lexeme) + 1;
			for(i = t; i > 0; i--)
				lexeme[i] = lexeme[i-1];
			lexeme[0] = aux;
		}
		symbol = getc(buffer);
		if(symbol == 39){
			t = strlen(lexeme);
			lexeme[t] = aux;
			lexeme[t+1] = '\0';
			return CADEIA;
		}
		else {
			ungetc(symbol,buffer);
			while(lexeme[i] != '\0'){
				 ungetc(lexeme[i],buffer);
				 i++;
			}
			return symbol;
		}
	}	
	/*
	 * se nao for nenhum dos tokens acima, retorna o proprio simbolo
	*/ 		
	else return symbol;
}

