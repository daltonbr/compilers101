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

Descriçaõ do modulo: emite rotulos e condições de desvio de um programa Pascal
**************************************************************************/

#include "global.h"
#include "rotulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*emite um desvio condicional para um rotulo. Esse desvio é encontrado usando a variavel "relacao"*/
void emitgo(label_t label){
if(strcmp(relacao,"=")==0)fprintf(buffer2,"\tjnz %d\n",label);
else if(strcmp(relacao,"<>")==0)fprintf(buffer2,"\tjz %d\n",label);
else if(strcmp(relacao,"<")==0)fprintf(buffer2,"\tjnb %d\n",label);
else if(strcmp(relacao,">")==0)fprintf(buffer2,"\tjna %d\n",label);
else if(strcmp(relacao,"<=")==0)fprintf(buffer2,"\tjb %d\n",label);
else if(strcmp(relacao,">=")==0)fprintf(buffer2,"\tja %d\n",label);
}

//emite um rotulo
void emitlabel(label_t label){
fprintf(buffer2,".L %d\n",label);
}
