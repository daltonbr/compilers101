/**************************************************************************
3§ Trabalho de compiladores
Autores: Jonni Borges Surano
	 Leonardo Mellaci de Matos	
Turma:	 3§BCC Integral
Prof.Dr. Eraldo Pereira Marinho
Finalidade do programa: Fazer um compilador Pascal em linguagem C, gerando um 
	programa em codigo de maquina.
Resumo: O usuario escolhe o arquivo que deseja compilar. O programa faz a compilacao 
  e armazena o codigo gerado em um arquivo .obj.
Compilador usado: Turbo C++
Obs: Foi usado o padrao INTEL, e nao o AT&T, na construcao do compilador

Descrição do modulo: Esse modulo é usado para fazer inserções e buscas na tabela de simbolos
**************************************************************************/


#include "global.h"
#include "simbolo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Insere um elemento na tabela de simbolos
void insert(){
indsimb++;
strcpy(tabsimb[indsimb].symbol,lexeme);
}

//encontra um elemento na tabela de simbolos
int encontra(string lex){
int i,ind;
for(i=0;i<=indsimb;i++){
                        if(strcmp(lex,tabsimb[i].symbol)==0)ind=i;
                        }
return ind;
}
