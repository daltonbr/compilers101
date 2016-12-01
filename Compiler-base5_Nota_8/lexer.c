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

Descrição do modulo: Esse modulo faz a analise semantica do compilador
**************************************************************************/

#include "lexer.h"
#include "simbolo.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Verifica se o proximo token e' o token esperado
token_t match(token_t token)
{
if(token==lookahead){
                     lookahead=gettoken();
                     return 1;
                     }
else{
     //caso nao seja, sai do programa
     printf("A expressao esta errada!!\n");
     exit(2);
     }
}

//converte o tipo do token atraves do nome do tipo por extenso
token_t converte(string tipos){
if(strcmp(tipos,"Var")==0)return IDVAR;
else if(strcmp(tipos,"Procedimento")==0)return IDROTINA;
else if(strcmp(tipos,"Funcao")==0)return IDFUNC;
else if(strcmp(tipos,"Const")==0)return IDCONST;
else if(strcmp(tipos,"Tipo")==0)return IDTIPO;
}


//encontra o proximo token
token_t gettoken(void)
{
int lexeme2,encont=0,i,indi;
while((lexeme2=getc(buffer)),isspace(lexeme2));
if(isalpha(lexeme2)){
                     //o token e um identificador
                     ungetc(lexeme2,buffer);
                     fscanf(buffer,"%[A-Za-z_0-9]",lexeme);
                     //o token e' "begin"
                     if(strcmp(lexeme,simbolo[0])==0)return BEGIN;
                     //o token e' "E"
                     else if(strcmp(lexeme,simbolo[1])==0)return E;
                     //o token e' "end"
                     else if(strcmp(lexeme,simbolo[2])==0)return END;
                     //o token e' "mod"
		     else if(strcmp(lexeme,simbolo[3])==0)return MOD;
                     //o token e' "and"
                     else if(strcmp(lexeme,simbolo[4])==0)return AND;
                     //o token e' "or"
                     else if(strcmp(lexeme,simbolo[5])==0)return OR;
                     //o token e' "not"
                     else if(strcmp(lexeme,simbolo[6])==0)return NOT;
	             //o token e' "if"
	             else if(strcmp(lexeme,simbolo[7])==0)return IF;
                     //o token e' "then"
                     else if(strcmp(lexeme,simbolo[8])==0)return THEN;
                     //o token e' "else"
                     else if(strcmp(lexeme,simbolo[9])==0)return ELSE;
                     //o token e' "case"
                     else if(strcmp(lexeme,simbolo[10])==0)return CASE;
                     //o token e' "of"
                     else if(strcmp(lexeme,simbolo[11])==0)return OF;
                     //o token e' "while"
                     else if(strcmp(lexeme,simbolo[12])==0)return WHILE;
                     //o token e' "do"
                     else if(strcmp(lexeme,simbolo[13])==0)return DO;
                     //o token e' "repeat"
                     else if(strcmp(lexeme,simbolo[14])==0)return REPEAT;
                     //o token e' "until"
                     else if(strcmp(lexeme,simbolo[15])==0)return UNTIL;
                     //o token e' "for"
                     else if(strcmp(lexeme,simbolo[16])==0)return FOR;
                     //o token e' "to"
                     else if(strcmp(lexeme,simbolo[17])==0)return TO;
                     //o token e' "const"
                     else if(strcmp(lexeme,simbolo[18])==0)return CONST;
                     //o token e' "type"
                     else if(strcmp(lexeme,simbolo[19])==0)return TYPE;
                     //o token e' "var"
                     else if(strcmp(lexeme,simbolo[20])==0)return VAR;
                     //o token e' "procedure"
                     else if(strcmp(lexeme,simbolo[21])==0)return PROCEDURE;
                     //o token e' "function"
                     else if(strcmp(lexeme,simbolo[22])==0)return FUNCTION;
                     //o token e' "goto"
                     else if(strcmp(lexeme,simbolo[23])==0)return GOTO;
                     //o token e' "false" ou "true"
                     else if((strcmp(lexeme,simbolo[24])==0)||(strcmp(lexeme,simbolo[25])==0))return IDCONST;
                     //o token e' "boolean","integer","char","real" ou "text"
                     else if((strcmp(lexeme,simbolo[26])==0)||(strcmp(lexeme,simbolo[27])==0)||(strcmp(lexeme,simbolo[28])==0)||(strcmp(lexeme,simbolo[29])==0)||(strcmp(lexeme,simbolo[30])==0))return IDTIPO;
                     //o token e' "input" ou "output"
                     else if((strcmp(lexeme,simbolo[31])==0)||(strcmp(lexeme,simbolo[32])==0))return IDVAR;
                     //o token e' "get","put","reset","rewrite","read" ou "write"
                     else if((strcmp(lexeme,simbolo[33])==0)||(strcmp(lexeme,simbolo[34])==0)||(strcmp(lexeme,simbolo[35])==0)||(strcmp(lexeme,simbolo[36])==0)||(strcmp(lexeme,simbolo[37])==0)||(strcmp(lexeme,simbolo[38])==0))return IDROTINA;
                     //o token e' "abs","sqr","odd","succ","pred","ord","chr","trunc","eof","sin","cos","exp","ln","sqrt" ou "arctan"
                     else if((strcmp(lexeme,simbolo[39])==0)||(strcmp(lexeme,simbolo[40])==0)||(strcmp(lexeme,simbolo[41])==0)||(strcmp(lexeme,simbolo[42])==0)||(strcmp(lexeme,simbolo[43])==0)||(strcmp(lexeme,simbolo[44])==0)||(strcmp(lexeme,simbolo[45])==0)||(strcmp(lexeme,simbolo[46])==0)||(strcmp(lexeme,simbolo[47])==0)||(strcmp(lexeme,simbolo[48])==0)||(strcmp(lexeme,simbolo[49])==0)||(strcmp(lexeme,simbolo[50])==0)||(strcmp(lexeme,simbolo[51])==0)||(strcmp(lexeme,simbolo[52])==0)||(strcmp(lexeme,simbolo[53])==0))return IDROTINA;
                     else{
                     	for(i=0;i<=indsimb;i++){
                        if(strcmp(lexeme,tabsimb[i].symbol)==0){
                        	//se entrar nesse laco, o identificador ja esta definido
                                indi=i;
				encont=1;
								}
							}
                        if(encont==0){
                                     //o identificador nao esta definido
                                     insert();
				     return ID;
                     		      }
                                      //retorna o identificador encontrado
                                      else return converte(tabsimb[indi].objtipo);
			}}
                     else if(isdigit(lexeme2))
                          {
                          //o token e uma constante numerica
                          ungetc(lexeme2,buffer);
                          fscanf(buffer,"%[0-9]",lexeme);
                          return CT;
                          }
                          else{
                          //o token e' "+" ou "-"
                          if((lexeme2=='+')||(lexeme2=='-')){
                                                             if(lexeme2=='+')strcpy(lexeme,"+");
                                                             else strcpy(lexeme,"-");
                                                             return ADD;}
                          //o token e' "*" ou "/"
                          else if((lexeme2=='*')||(lexeme2=='/')){
                                                                  if(lexeme2=='*')strcpy(lexeme,"*");
                                                                  else strcpy(lexeme,"/");
                                                                  return MUL;}
                          //o token e' "("
                          else if(lexeme2=='(')return PAR1;
                          //o token e' ")"
                          else if(lexeme2==')')return PAR2;
                          //o token e' "{"
                          else if(lexeme2=='{')return CHAV1;
                          //o token e' "}"
                          else if(lexeme2=='}')return CHAV2;
                          //o token e' "`"
                          else if(lexeme2=='`')return ASPA;
                          else if(lexeme2==':')
                               {
                               lexeme2=getc(buffer);
                               if(lexeme2=='='){
                                                //o token e' ":="
			       			strcpy(lexeme,":=");
			       			return ASSIGN;
						}
                               else {
                                     //o token e' ":"
			       	     ungetc(lexeme2,buffer);
				     strcpy(lexeme,":");
				     return DPTO;}
                               }   
                          //o token e' "."
                          else if(lexeme2=='.')return PONTO;
                          //o token e' ";"
                          else if(lexeme2==';')return PTVIRG;
                          //o token e' ","
                          else if(lexeme2==',')return VIRG;
                          //o token e' "="
                          else if(lexeme2=='=')return IGUAL;
                          else if(lexeme2=='<'){
			  			lexeme2=getc(buffer);
						if((lexeme2!='>')&&(lexeme2!='=')){
                                                                                   //o token e' "<"
										   ungetc(lexeme2,buffer);
										   strcpy(lexeme,"<");
										   }
                                                //o token e' "<="
                                                else if(lexeme2=='=')strcpy(lexeme,"<=");
                                                //o token e' "<>"
						else strcpy(lexeme,"<>");
                                                //se o token for "<>,"<=",">=","<" ou ">", e' um operador relacional
                                                return RELAC;
						}
                          else if(lexeme2=='>'){
			  			lexeme2=getc(buffer);
						if(lexeme2!='='){
                                                                 //o token e' ">="
								 ungetc(lexeme2,buffer);
								 strcpy(lexeme,">");
								 }
                                                //o token e' ">"
						else strcpy(lexeme,">");
                                                //se o token for "<>,"<=",">=","<" ou ">", e' um operador relacional
                                                return RELAC;
						}}
}
