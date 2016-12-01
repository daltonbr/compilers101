#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "lexan.h"
#include "symbol.h"
#include "parser.h"
	
int main(void){
	/*
	 * Verifica se o arquivo buffer, de onde serao lidas as cadeias de
	 * caracteres foi aberto com sucesso
	*/
	if (!(buffer = fopen("teste1.pas", "r"))) {
		perror("teste1.pas");
		return errno;
	}
	/*
	 * O codigo em linguagem de montagem sera salvo em  "codigo.s"
	*/
	if (!(arq = fopen("codigo.s","w"))){
		perror("codigo.s");
		return errno;
	}	
	/*
	 * Insere as palavras reservadas na tabela de simbolos
	*/
	reservadas();	
	lookahead=gettoken();
	trypascal();	
	return 0;	
}
