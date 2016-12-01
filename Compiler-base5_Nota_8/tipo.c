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

Descrição do modulo: Esse modulo contem as funções usadas na verificação de tipos
**************************************************************************/

#include "global.h"
#include "tipo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//exibe mensagem de erro e sai do programa
void erro(){
printf("Erro de tipo");exit(1);}

//verifica se o tipo de uma operacao e' valido
void tipoop(int tipo1){
if(tipoa==-1)tipoa=tipo1;
else if(tipoa==0){
	if(tipo1!=0)erro();
	}
else if(tipoa==3){
	if(tipo1!=3)erro();
	}
else if(tipoa==2){
	if(tipo1!=2)erro();
	}
else if(tipoa==1){
	/*tipo atual é inteiro*/
	if((tipo1!=3)&&(tipo1!=1))erro();
	else if(tipo1==3)tipoa=3;
	}
else if(tipoa==4){
	if(tipo1!=4)erro();
	}
}

//verifica se uma atribuicao pode ser feita
void tipoat(int tipo1){
if((!(tipo1==3)&&(tipoa==1))&&(tipo1!=tipoa))erro();
else tipoa=-1;
}

/*converte um tipo escrito por extenso para um valor inteiro*/
/*Os tipos sao definidos da seguinte forma:
0-Boolean
1-Inteiro
2-Char
3-Real
4-Text*/
int convtipo(string tipo1){
if(strcmp(tipo1,"boolean")==0)return 0;
else if(strcmp(tipo1,"integer")==0)return 1;
else if(strcmp(tipo1,"char")==0)return 2;
else if(strcmp(tipo1,"real")==0)return 3;
else if(strcmp(tipo1,"text")==0)return 4;
else if(isdigit(tipo1[0]))return 3;
else if(!(isspace(tipo1[1])))return 4;
else return 2;
}
