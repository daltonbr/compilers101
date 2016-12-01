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

Descrição do modulo: Faz a analise sintatica do compilador
**************************************************************************/

#include "global.h"
#include "lexer.h"
#include "parser.h"
#include "tipo.h"
#include "rotulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//retorna o tamanho de um tipo
int tamanho(string tipodef){
int i;
if(strcmp(tipodef,"boolean")==0)return 5;
else if(strcmp(tipodef,"integer")==0)return 4;
else if(strcmp(tipodef,"char")==0)return 1;
else if(strcmp(tipodef,"real")==0)return 8;
else if(strcmp(tipodef,"text")==0)return 255;
else for(i=0;i<=indsimb;i++){
	if(strcmp(tabsimb[indsimb].objtipo,"Tipo")==0)
        	return tabsimb[indsimb].offset;}
}

//se for um procedimento, retorna a posicao relativa a ele na pilha
void troca(string nome1){
int i,encont=0;
for(i=0;i<=indtroca&&encont==0;i++){
	if(strcmp(trocas[indtroca][0],nome1)==0){
			encont=1;
			strcpy(nome1,trocas[indtroca][1]);
			}
	}
}

//Parser baseado na definicao da linguagem Pascal de Wirth

//Funcao que encontra um numero sem sinal em Pascal
void num_sem_sinal()
{
float total;
int multi=1,inteiro=1,total2,i;
string aux;
strcpy(aux,lexeme);
total=atof(lexeme);
match(CT);
if(lookahead==PONTO){
                     inteiro=0;
		     match(PONTO);
		     for(i=0;i<=strlen(lexeme);i++)
                     total+=atof(lexeme)/10;
	             match(CT);
		     }
if(lookahead==E){
		match(E);
		if(lookahead==ADD){
                                   if(lexeme=="-")multi=0;
				   match(ADD);
				   }
		/*Se for exponenciação, calcula o valor relativo a ela*/
		if(multi==1){for(i=0;i<=(atoi(lexeme));i++)total=total*10;}
                else {for(i=0;i<=(atoi(lexeme));i++)total=total/10;}
		match(CT);
                }
 if(inteiro==1)tipoop(convtipo("integer"));
 else tipoop(convtipo("real"));
 /*total2=int(total);*/
 /*armazena o resultado em %eax*/
 fprintf(buffer2,"\tmov %%eax, $%d\n",total);
}

//Funcao que encontra uma constante sem sinal em Pascal
void cons_sem_sinal(){
if(lookahead==ASPA){
		    /*Operação realizada através de strings*/
		    match(ASPA);fprintf(buffer2,"\tmov %%esi, '%s'\n",lexeme);
		    fprintf(buffer2,"\tldos");
                    tipoop(convtipo("text"));
		    if(lookahead==ID){
                                      //indsimb e' incrementado no lexer. Isso nao deve ser feito porque o identificador nao e' um identificador padrao
		    		      indsimb--;
				      match(ID);
				      }
                    //senao pode ser qualquer outro token
		    else match(lookahead);
		    match(ASPA);
		    }
else if(lookahead==CT)num_sem_sinal();
else {
       /*armazena o resultado em %eax*/
       /*operação realizada sobre strings*/
       fprintf(buffer2,"\tmov %%esi, '%s'\n",lexeme);
       fprintf(buffer2,"\tldos");
       tipoop(convtipo("boolean"));
       match(IDCONST);
       }
}

//Funcao que encontra uma constante em Pascal
void constante()
{
int aux;
string straux;
if(lookahead==ASPA){
		    /*operação realizada sobre strings*/
		    match(ASPA);
                    tipoop(convtipo("text"));
                    fprintf(buffer2,"\tmov %%esi, '%s'\n",lexeme);
		    fprintf(buffer2,"\tldos");
		    //armazena o resultado em %eax
		    if(lookahead==ID){
                                      //indsimb e' incrementado no lexer. Isso nao deve ser feito porque o identificador nao e' um identificador padrao
		    		      indsimb--;
				      match(ID);
				      }
                    //senao pode ser qualquer outro token
		    else match(lookahead);
		    match(ASPA);
		    }
else {
	if(lookahead==ADD){
                           if(lexeme=="-")aux=1;
                           match(ADD);
                           }
	if(lookahead==CT){
			num_sem_sinal();
                        if(aux==1)fprintf(buffer2,"\tneg %%eax\n");
                          }
	else {
                if(aux==1){
                	strcpy(straux,"-");
                	strcat(straux,lexeme);
                        strcpy(lexeme,straux);}
                //armazena o resultado em %eax
		/*operação sobre string*/
                fprintf(buffer2,"\tmov %%esi, '%s'\n",lexeme);
                fprintf(buffer2,"\tldos");
                tipoop(convtipo("boolean"));
		match(IDCONST);
	     }
     }
}


//Funcao que encontra um fator em Pascal
void fator()
{
string aux,aux2;
int indice;
if(lookahead==PAR1){
		    match(PAR1);
                    compare=0;
		    expressao();
		    match(PAR2);
		    }
else if(lookahead==NOT){
			match(NOT);
			fator();
			/*realiza a operação not*/
                        fprintf(buffer2,"\tnot %%eax\n");
			}
else if(lookahead==IDVAR){
                          strcpy(aux2,lexeme);
                          troca(lexeme);
                          strcpy(aux,lexeme);
                          strcpy(lexeme,aux2);
                          indice=encontra(lexeme);
                          if((strcmp(tabsimb[indice].tipo,"text")==0)||(strcmp(tabsimb[indice].tipo,"char")==0)||(strcmp(tabsimb[indice].tipo,"boolean")==0))
                              fprintf(buffer2,"\tmov %esi,%s\n\tldos",aux);
                          else fprintf(buffer2,"\tmov %%eax, %s\n",aux);
                          tipoop(convtipo(tabsimb[indice].tipo));
			  match(IDVAR);}
else if(lookahead==IDFUNC){
                           strcpy(aux,lexeme);
                           indice=encontra(lexeme);
                           tipoop(convtipo(tabsimb[indice].tipo));
                           match(IDFUNC);
			   if(lookahead==PAR1){
					       match(PAR1);
					       expressao();
                                               fprintf(buffer2,"\tpush %%eax\n");
					       while(lookahead==VIRG){
					       			      match(VIRG);
								      expressao();
			                                              fprintf(buffer2,"\tpush %%eax\n");
								      }
					       match(PAR2);
					       }
			  /*chama uma funcao*/
                          fprintf(buffer2,"\tcall %s\n",aux);
			  }
else cons_sem_sinal();
}

//Funcao que encontra um termo em Pascal
void termo(){
string sinal;
fator();
while((lookahead==MUL)||(lookahead==MOD)||(lookahead==AND)){
                        strcpy(sinal,lexeme);
                        fprintf(buffer2,"\tmov %%ebx, %%eax\n");
			match(lookahead);
			fator();
                        if(strcmp(lexeme,"*")==0)/*realiza uma multiplicação*/
					      fprintf(buffer2,"\tmul %%ebx\n");
                        else if(strcmp(lexeme,"/")==0)/*realiza uma divisao*/
					     {fprintf(buffer2,"\txchg %%eax %%ebx\n");
					     fprintf(buffer2,"\tdiv %%ebx\n");}
                        else if(strcmp(lexeme,"mod")==0)/*encontra o resto da divisão*/
					       {fprintf(buffer2,"\txchg %%eax %%ebx\n");
					       fprintf(buffer2,"\tdiv %%eax %%ebx\n");
					       fprintf(buffer2,"\tmov %%eax %%edx\n");}
                        else if(strcmp(lexeme,"and")==0)/*realiza uma operação and*/
							 {fprintf(buffer2,"\txchg %%eax %%ebx\n");
							 fprintf(buffer2,"\tand %%eax %%ebx\n");}
			}
//o resultado e' armazenado %eax
}

//Funcao que encontra uma expressao simples em Pascal
void exp_simples(){
string sinal;
if(lookahead==ADD){
                   strcpy(sinal,lexeme);
		   match(ADD);
		   }
termo();
if(sinal=="-")fprintf(buffer2,"\tneg %%eax\n");
while((lookahead==ADD)||(lookahead==OR)){
                                         strcpy(sinal,lexeme);
                                         fprintf(buffer2,"\tmov %%ebx,%%eax\n");
					 match(lookahead);
					 termo();
                                         fprintf(buffer2,"\txchg %%eax %%ebx\n");
                                         if(strcmp(sinal,"+")==0)/*realiza uma adição*/
					 	fprintf(buffer2,"\tadd %%eax %%ebx\n");
                                         else if(strcmp(sinal,"-")==0)/*realiza uma subtração*/
					 	fprintf(buffer2,"\tsub %%eax %%ebx\n");
                                         else if(strcmp(sinal,"or")==0)/*realiza uma operação or*/
					 	fprintf(buffer2,"\tor %%eax %%ebx\n");
                                         }
//o resultado e' armazenado %eax
}

//Funcao que encontra uma expressao em Pascal
void expressao(){
exp_simples();
if(lookahead==RELAC){
                     strcpy(relacao,lexeme);
                     fprintf(buffer2,"\tmov %%ebx, %%eax\n");
		     match(RELAC);
		     exp_simples();
                     fprintf(buffer2,"\txchg %%eax %%ebx\n");
                     if(compare==1)fprintf(buffer2,"\tcmp %%eax %%ebx\n");
		     }
else if(lookahead==IGUAL){
                          strcpy(relacao,"=");
                          fprintf(buffer2,"\tmov %%ebx, %%eax\n");
			  match(IGUAL);
			  exp_simples();
                          fprintf(buffer2,"\txchg %%eax %%ebx\n");
                          //compare define se deve haver comparacao ou nao
                          if(compare==1)fprintf(buffer2,"\tcmp %%eax %%ebx\n");
			  }
//o resultado e' armazenado %eax
}

//Funcao que encontra uma lista de parametros em Pascal
void lista(){
string aux;
argumentos=-1;
/*define o numero de argumentos  inicial com -1*/
/*nvar representa o numero de parametros da subrotina*/
/*offs representa o offset atual em relação à pilha*/
int offs=8,nvar,i;
if(lookahead==PAR1){
		    nvar=0;
		    do{
                        argumentos++;
			match(lookahead);
			if(lookahead==VAR)match(VAR);
                                                nvar++;
						/*define o tipo do objeto como variavel*/
						strcpy(tabsimb[indsimb].objtipo,"Var");
						match(ID);
	 					while(lookahead==VIRG){
                                                                        argumentos++;
									match(VIRG);
                                                                        /*identificador encontrado e' definido como um identificador de variavel*/
									/*define o tipo do objeto como variavel*/
                                                                        strcpy(tabsimb[indsimb].objtipo,"Var");
                                                                        nvar++;
									match(ID);
									}
	 					match(DPTO);
						for(i=1;i<=nvar;i++){
							strcpy(tabsimb[indsimb-nvar+i].tipo,lexeme);
                                                        tabsimb[indsimb-nvar+i].offset=offs;
							/*equivale a função itoa*/
							sprintf(aux,"%d",offs);
							offs+=tamanho(lexeme);
                                                        indtroca++;
                                                        strcpy(trocas[indtroca][0],tabsimb[indsimb-nvar+i].symbol);
                                                        strcat(aux,"(%ebp)");
                                                        strcpy(trocas[indtroca][1],aux);}
                                                match(IDTIPO);


		}while(lookahead==PTVIRG);
		match(PAR2);
	}
}

//Funcao que encontra um comando  em Pascal
void comando(){
int L_init,L_fim,tipo2,indice;
string aux;
if(lookahead==CT){
		  /*insere um rotulo*/
		  strcat(lexeme,"Goto");
                  fprintf(buffer2,".L %s\n",lexeme);
		  match(CT);
		  match(DPTO);
		  }
if(lookahead==BEGIN){
                     match(BEGIN);
		     comando();
		     while(lookahead==PTVIRG){
					      match(PTVIRG);
					      comando();
					      }
		     match(END);
		     }
else if(lookahead==IF){
			/*executa um laço If-Then-Else*/
			match(IF);
                        compare=1;
			expressao();
                        tipoa=-1;
                        emitgo(L+1);
                        emitlabel(L);
                        L++;
			match(THEN);
                        L_init=L;
                        L++;
			comando();
			if(lookahead==ELSE){
                                            L_fim=L;
                                            L++;
                                            fprintf(buffer2,"\tjmp %d\n",L_fim);
                                            emitlabel(L_init);
                                            match(ELSE);
                                            comando();
					    }
			emitlabel(L_fim);
                        L++;
			}
else if(lookahead==CASE){
			 /*executa um laço CASE*/
                         L_fim=L;
			 match(CASE);
                         compare=1;
			 expressao();
                         tipo2=tipoa;
                         tipoa=-1;
			 do{
                             L++;
                             match(lookahead);
                             fprintf(buffer2,"\tpush %%eax\n");
                             /*tem q comparar a cte com eax*/
                             constante();
                             tipoat(tipo2);
                             fprintf(buffer2,"\tmov %%ebx, %%eax\n");
                             fprintf(buffer2,"\tpop %%eax\n");
                             fprintf(buffer2,"\tpush %%eax\n");
                             fprintf(buffer2,"\tcmp %%eax, %%ebx\n");
                             strcpy(relacao,"=");
                             emitgo(L+1);
                             L_init=L+1;
                             match(DPTO);
                             comando();
                             fprintf(buffer2,"\tjmp %d\n",L_fim);
                             emitlabel(L_init);
	  		 }while(lookahead==PTVIRG);
                         emitlabel(L_fim);
                         fprintf(buffer2,"\tpop %%eax");
                         L+=2;
                         match(END);
			 }
else if(lookahead==WHILE){
			  /*executa um laço While-Do*/
			  L_fim=L;
                          L++;
                          L_init=L;
                          emitlabel(L);
                          L++;
			  match(WHILE);
                          compare=1;
			  expressao();
                          tipoa=-1;
                          emitgo(L_fim);
                          match(DO);
			  comando();
                          fprintf(buffer2,"\tjmp %d\n",L_init);
                          emitlabel(L_fim);
                          }
else if(lookahead==REPEAT){
			   /*executa um laço Repeat-Until*/
			   match(REPEAT);
                           L_init=L;
                           emitlabel(L);
                           L++;
			   comando();
		           while(lookahead==PTVIRG){

						    match(PTVIRG);
						    comando();
						    }
                           match(UNTIL);
                           compare=1;
			   expressao();
                           tipoa=-1;
                           emitgo(L_init);
                           }
else if(lookahead==FOR){
			/*executa um laço For*/
                        match(FOR);
                        indice=encontra(lexeme);
                        tipo2=convtipo(tabsimb[indice].tipo);
			match(IDVAR);
			match(ASSIGN);
                        compare=0;
			expressao();
                        tipoat(tipo2);
                        fprintf(buffer2,"\tmov %%ecx, %%eax\n");
			match(TO);
	 		expressao();
                        fprintf(buffer2,"\tsub %%eax, %%ecx\n");
                        fprintf(buffer2,"\tmov %%ecx, %%eax\n");
                        L_init=L;
                        L++;
			emitlabel(L_init);
                        match(DO);
			comando();
			/*utiliza a instrução loop, que decrementa automaticamente o contador %ecx*/
                        fprintf(buffer2,"\tloop %d\n",L_init);
			}
else if(lookahead==GOTO){
			 /*executa um salto Goto*/
                         strcat(lexeme,"Goto");
                         match(GOTO);
			 fprintf(buffer2,"\tjmp %s\n",lexeme);
			 match(CT);
			 }
else if(lookahead==IDFUNC){
			   /*chama uma função e atribui um valor a ela*/
                           indice=encontra(lexeme);
                           tipo2=convtipo(tabsimb[indice].tipo);
                           strcpy(aux,lexeme);
			   match(IDFUNC);
			   match(ASSIGN);
                           compare=0;
			   expressao();
                           tipoat(tipo2);
                           fprintf(buffer2,"\tmov %s, %%eax\n",aux);
			   }
else if(lookahead==IDVAR){
			  /*encontra uma variavel e atribui a ela um valor*/
                          indice=encontra(lexeme);
                          tipo2=convtipo(tabsimb[indice].tipo);
                          troca(lexeme);
                          strcpy(aux,lexeme);
			  match(IDVAR);
                          match(ASSIGN);
                          compare=0;
			  expressao();
                          tipoat(tipo2);
                          fprintf(buffer2,"\tmov %s, %%eax\n",aux);
			  }
else if(lookahead==IDROTINA){
			     /*executa um procedimento*/
                             strcpy(aux,lexeme);
			     match(IDROTINA);
			     if(lookahead==PAR1){
						 match(PAR1);
                                                 compare=0;
						 expressao();
                                                 tipoa=-1;
                                                 fprintf(buffer2,"\tpush %%eax\n");
						 while(lookahead==VIRG){
						 			match(VIRG);
									expressao();
                                                                        tipoa=-1;
                                                                        fprintf(buffer2,"\tpush %%eax\n");
									}
						 match(PAR2);
						 }
                             fprintf(buffer2,"\tcall %s\n",aux);
			     }

}

//Funcao que encontra um bloco em Pascal
void bloco()
{
int varcont,i,offs=0,indaux,indaux2;
string aux,nome;

if(lookahead==VAR){
                   match(VAR);
		   /*so escreve .secton .bss se estiver no programa principal*/
		   if(princ==1){fprintf(buffer2,".section.bss\n");
                   do{
                       varcont=0;
                       //identificador encontrado e' definido como um identificador de variavel
		       strcpy(tabsimb[indsimb].objtipo,"Var");
		       match(ID);
		       while(lookahead==VIRG){
                      			      varcont++;
		       			      match(VIRG);
                                              //identificador encontrado e' definido como um identificador de variavel
					      strcpy(tabsimb[indsimb].objtipo,"Var");
					      match(ID);
					      }
	  	       match(DPTO);
                       for(i=varcont;i>=0;i--){
                       strcpy(tabsimb[indsimb-i].tipo,lexeme);
		       /*declara uma variavel global*/
                       fprintf(buffer2,".common %s, %d\n",tabsimb[indsimb-i].symbol,tamanho(tabsimb[indsimb-i].tipo));}
		       match(IDTIPO);

		       match(PTVIRG);

	  	   }while(lookahead==ID);
                   princ=0;
                   }
                   else{
                   do{
                       varcont=0;
                       //identificador encontrado e' definido como um identificador de variavel
		       strcpy(tabsimb[indsimb].objtipo,"Var");
		       match(ID);
		       while(lookahead==VIRG){
                      			      varcont++;
		       			      match(VIRG);
                                              //identificador encontrado e' definido como um identificador de variavel
					      strcpy(tabsimb[indsimb].objtipo,"Var");


					      match(ID);
					      }
	  	       match(DPTO);
		       /*atualiza o offset*/
                       offs=-4*nivel;
                       for(i=varcont;i>=0;i--){
                       strcpy(tabsimb[indsimb-i].tipo,lexeme);
                       indtroca++;
                       strcpy(trocas[indtroca][0],tabsimb[indsimb-i].symbol);
                       offs-=tamanho(tabsimb[indsimb-i].tipo);
		       /*função equivalente ao itoa*/
		       sprintf(aux,"%d",offs);
                       strcat(aux,"(%ebp)");
                       strcpy(trocas[indtroca][1],aux);

		       }
		       match(IDTIPO);

		       match(PTVIRG);

	  	   }while(lookahead==ID);


			}
		   }
while((lookahead==PROCEDURE)||(lookahead==FUNCTION)){
/*é um subprograma*/
                                nivel++;
                                if(lookahead==PROCEDURE){
							 indaux2=indtroca;
							 match(PROCEDURE);
                                                         indaux=indsimb;
                                                         //identificador encontrado e' definido como um identificador de rotina
							 strcpy(tabsimb[indsimb].objtipo,"Procedimento");
                                                         strcpy(aux,lexeme);
                                                         strcpy(nome,"ret");
                                                         strcat(nome,aux);
                                                         match(ID);
                                                         fprintf(buffer2,"\tjmp %s\n",nome);
                                                         fprintf(buffer2,"%s:\n",aux);
                                                         fprintf(buffer2,".section .text\n.global .%s\n.type .%s,@function\n",aux,aux);
                                                         fprintf(buffer2,"\tpush %%ebp\n");
                                                         fprintf(buffer2,"\tmov %%ebp,%%esp\n");
                                                         fprintf(buffer2,"\tadd %%esp, $-4\n");
							 lista();
							 match(PTVIRG);
							 bloco();
							 match(PTVIRG);
                                                         fprintf(buffer2,"\tmov %%esp, %%ebp\n");
                                                         fprintf(buffer2,"\tpop %%ebp\n");
                                                         fprintf(buffer2,"\tret\n");
                                                         fprintf(buffer2,".L %s:\n",nome);
                                                         indsimb=indaux;
                                                         indtroca=indaux2;
                                                         nivel--;

							 }
				else{

							 match(FUNCTION);
                                                         indaux=indsimb;
                                                         strcpy(aux,lexeme);
                                                         strcpy(nome,"ret");
                                                         strcat(nome,aux);
							 //identificador encontrado e' definido como um identificador de funcao
							 strcpy(tabsimb[indsimb].objtipo,"Funcao");
							 match(ID);
                                                         fprintf(buffer2,"\tjmp %s\n",nome);
                                                         fprintf(buffer2,"%s:\n",aux);
                                                         fprintf(buffer2,".section .text\n.global .%s\n.type .%s,@function\n",aux,aux);
                                                         fprintf(buffer2,"\tpush %%ebp\n");
                                                         fprintf(buffer2,"\tmov %%ebp,%%esp\n");
                                                         fprintf(buffer2,"\tadd %%esp, $-4\n");
							 lista();
							 match(DPTO);
                                                         strcpy(tabsimb[indaux].tipo,lexeme);
                                                         match(IDTIPO);
							 match(PTVIRG);
							 bloco();
                                                         match(PTVIRG);
                                                         fprintf(buffer2,"\tmov %%esp, %%ebp\n");
                                                         fprintf(buffer2,"\tpop %%ebp\n");
                                                         fprintf(buffer2,"\tret\n");
                                                         fprintf(buffer2,".L %s:\n",nome);
                                                         indsimb=indaux;
                                                         indtroca=-1;
                                                         nivel--;
                                                         }
}
if(nivel==0)fprintf(buffer2,".global _start\n_start:\n");
match(BEGIN);comando();
while(lookahead==PTVIRG){

			 match(PTVIRG);
			 comando();
			 }
match(END);


}

//Funcao que encontra um programa em Pascal
void programa()
{
bloco();
fprintf(buffer2,"int $128\n\n");
match(PONTO);
}


/*Funcao que cria as rotinas read e write no final do arquivo*/
void rotinas(){
fprintf(buffer2,"write:\n.section .text\n.global _write\n.type _write,@function\n");
fprintf(buffer2,"\tmov %%ecx, 8(%%ebp)\n");
fprintf(buffer2,"\tmov %%eax, $4\n");
fprintf(buffer2,"\tmov %%ebx, $1\n");
fprintf(buffer2,"\tint $128\n");
fprintf(buffer2,"\tret\n");
fprintf(buffer2,"read:\n.section .text\n.global _read\n.type _read,@function\n");
fprintf(buffer2,"\tmov %%eax, $3\n");
fprintf(buffer2,"\tmov %%ebx, $0\n");
fprintf(buffer2,"\tlea %%ecx, 8(%%ebp)\n");
fprintf(buffer2,"\tint $128\n");
fprintf(buffer2,"\tret\n");
}
