#include <stdio.h>
#include <string.h>
#include "lexan.h"

FILE *arq;
int param;
int pos;
char arg[16][256];
/*
 * str sera utilizada para armazenar temporariamente instrucoes que posteriormente seram concatendas
 * a arg[n]
 */
char str[256];
/*
 * este procedimento cria o codigo em linguagem de montagem para os operadores
 * relacionais
*/
void operador_relacional(token_t look){
	fprintf(arq,"\n\t popl %%eax");
	fprintf(arq,"\n\t cmpl %%eax,(%%esp)");
	switch(look){
		case OP_DIF:
			fprintf(arq,"\n\t setne %%al");
			break;
		case OP_LE:
			fprintf(arq,"\n\t setle %%al");
			break;
		case OP_LT:
			fprintf(arq,"\n\t setl %%al");
			break;
		case OP_ME:
			fprintf(arq,"\n\t setge %%al");
			break;
		case OP_MT:
			fprintf(arq,"\n\t setg %%al");
			break;
		case OP_EQ:
			fprintf(arq,"\n\t sete %%al");
			break;
	}
}

/*
 * este procedimento escreve o codigo em linguagem de montagem para as rotinas
 * de leitura do teclado e escrita na monitor
*/
void leitura_escrita(token_t look,char *nome1,float comp){
	switch(look){
		/*
		 * leitura do teclado
		 */
		case READ:
			fprintf(arq,"\n\t movl &3,%%eax");
			fprintf(arq,"\n\t xorl %%ebx, %%ebx");
			fprintf(arq,"\n\t movl %s,%%ecx",nome1);
			fprintf(arq,"\n\t movl %f,%%edx",comp);
			fprintf(arq,"\n\t int &128");
			break;
		case WRITE:
			fprintf(arq,"\n\t movl &4,%%eax");
			fprintf(arq,"\n\t movl &1,%%ebx");
			fprintf(arq,"\n\t movl %s,%%ecx",nome1);
			fprintf(arq,"\n\t movl %f,%%edx",comp);
			break;
	}
}

/*
 * este procedimento gera o codigo em linguagem assembly 
 * para  a soma ou a subtracao de termos 
 */
void cod_soma(token_t look){	
	if(param==0){
		fprintf(arq,"\n\t popl %%eax");
		if(look == OP_ADD) fprintf(arq,"\n\t addl (%%esp),%%eax");
		else fprintf(arq,"\n\t subl (%%esp),%%eax");	
		/*
	 	 * desempilha o operando que ainda estava na pilha
	 	*/
		fprintf(arq,"\n\t addl &4,%%esp");	
	}
	else{	/*
	   	 * se for argumento de funcao ou procedimento salva em arg[n] para escrever
		 * na ordem certa no arquivo *.s 
		 */	   
		if(look==OP_ADD) {
			sprintf(str,"%s","\n\t popl %eax\n\t addl (%esp),%eax \n\t addl &4,%esp");
			strcat(arg[pos],str);
		}	
		else {
			sprintf(str,"%s","\n\t popl %eax \n\t subl (%esp),%eax \n\t addl &4,%esp");		
			strcat(arg[pos],str);
		}	
	}	
}

/*
 * este procedimento gera o codigo em linguagem de montagem para a multiplicacao
 */
void cod_multiplicacao(void){
	if(param==0){		
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t imul (%%esp)");
		/*
	 	* "desempilha" o valor da multiplicacao
	 	*/
		fprintf(arq,"\n\t addl &4,%%esp");	
	}
	else{	
		/* 
		 * salva em arg[n] se for argumento de procedimento ou funcao
		 */	
		sprintf(str,"%s","\n\t popl %%eax \n\t imul (%esp) \n\t addl &4,%esp");
		strcat(arg[pos],str);
	}	
}

/*
 * este procedimento gera o codigo em linguagem de montagem para a divisao
 */
void cod_divisao(void){
	if(param==0){
		fprintf(arq,"\n\t popl %%ecx");
		fprintf(arq,"\n\t popl %%eax");	
		fprintf(arq,"\n\t cltd");
		fprintf(arq,"\n\t idivl %%ecx");	
	}
	else{
		/*
		 * salva em arg[n] se for argumento de procedimento ou funcao
		 */
		sprintf(str,"%s","\n\t popl %ecx \n\t popl %eax \n\t cltd \n\t idivl %ecx");
		strcat(arg[pos],str);
	}	
}

/*
 * este procedimento gera o codigo em linguagem de montagem para o comando NOT
 */
void cod_not(void){
	if(param==0){
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t xorb &1,%%al");
	}
	else{
		/*
		 * salva em arg[n] se for argumento de procedimento ou funcao
		 */
		sprintf(str,"%s","\n\t popl %eax \n\t xorb &1,%%al");
		strcat(arg[pos],str);
	}	
}

/*
 * este procedimento gera o codigo em linguagem assembly para o processo 
 * de empilhar um valor na pilha quando o escopo e o programa principal
 */
void cod_empilha(token_t look){
	
	if(look == NUM_I ){
		if(param==0){
			fprintf(arq,"\n\t movl &%d,%%eax",num1);
			fprintf(arq,"\n\t pushl %%eax");
		}
		else {	
			/*
			 * salva em arg[n] se for argumento
			 */		
			sprintf(str,"\n\t movl &%d,%ceax \n\t pushl %ceax",num1,'%','%');	
			strcat(arg[pos],str);
		}	
	}	
	else if(look == NUM_R){
		if(param ==0){
			fprintf(arq,"\n\t movl &%f,%%eax",lexnum);
			fprintf(arq,"\n\t pushl %%eax");
		}
		else{	
			/*
			 * salva em arg[n] se for argumento
			 */
			sprintf(str,"\n\t movl &%f,%ceax",lexnum,'%');
			strcat(arg[pos],str);
		}	
	}	
	else if (look == ID){
		if(param==0){			
			fprintf(arq,"\n\t movl %s,%%eax",lexeme);
			fprintf(arq,"\n\t pushl %%eax");
		}
		else {
			/*
			 * salva em arg[n] se for argumento
			 */
			sprintf(str,"\n\t movl %s,%ceax \n\t pushl %%eax",lexeme,'%');
			strcat(arg[pos],str);
		}
	}
}

/*
 * este procedimento gera o codigo em linguagem assembly para utilizar valores
 * que ja estao na pilha e que sao argumentos de procedimentos e funcoes
 */
void cod_empilha2(int n){
	fprintf(arq,"\n\t movl %d(%%ebp),%%eax",n);
	fprintf(arq,"\n\t pushl %%eax");
}

/*
 * funcoes para emissoes de rotulos
 */
void emitLabel(int label){
	fprintf(arq,"\n.L%d: \n",label);
}

void emitGofalse(int label){
	fprintf(arq,"\n\t jz .L%d",label);
}

void emitGotrue(int label){
	fprintf(arq,"\n\t jnz .L%d",label);
}

void emitGoto(int label){
	fprintf(arq,"\n\t jmp .L%d",label);
}

/*
 * este procedimento gera o codigo em linguagem assembly
 * para os comandos DIV e MOD
 */
void cod_mod_div(token_t look){
	if(param==0){
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t popl %%ebx");
		fprintf(arq,"\n\t idiv %%ebx");
		if(look == DIV)
			fprintf(arq,"\n\t pushl %%eax");

		else if (look == MOD)
			fprintf(arq,"\n\t pushl %%edx");
	}
	/*
	 * salva em arg[n] se for argumento
	 */
	else if(look==DIV) {
			sprintf(str,"%s","\n\t popl %eax \n\t popl %ebx \n\t idivl %ebx \n\t pushl %eax");
			strcat(arg[pos],str);
		}
		else if(look == MOD) {
			sprintf(str,"%s","\n\t popl %eax \n\t popl %ebx \n\t idivl %ebx \n\t pushl %%edx");
			strcat(arg[pos],str);
		}
}

/*
 * este procedimento gera o codigo em linguagem assembly para
 * os comandos AND e OR
 */
void cod_and_or(token_t look){
	if(param==0){
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t popl %%ebx");
		if(look == AND) fprintf(arq,"\n\t andb %%ebx, %%al");
		else if (look == OR) fprintf(arq,"\n\t orb %%ebx, %%al");
	}
	/*
	 * salva em arg[n] se for argumento
	 */
	else if(look == AND) {
			sprintf(str,"%s","\n\t popl %eax\n\t popl %ebx andb %%ebx,%%al");
			strcat(arg[pos],str);
		}
		else {
			sprintf(str,"%s","\n\t popl %eax\n\t popl %ebx\n\t orb %%ebx,%%al");
			strcat(arg[pos],str);
		}
}

/*
 * este procedimento verifica a compatibilidade de tipos
 */
int verifica_tipo(int t1, int t2){
	if (t1==INT && t2==INT) return INT;
	else if((t1==RL && t2==INT) || (t1==INT && t2==RL) || (t1==RL && t2 == RL)) return RL;
	else if (t1==BLN && t2==BLN) return BLN;
	else if(t1==CH && t2==CH) return CH;
	else return err_tipo;
}
/*
 * este procedimento gera o codigo em assembly para empilhar o acumulador
 * que sempre possui o resultado de uma expressao
 */
void empilha_acc(void){

	if(param==0) fprintf(arq,"\n\t pushl %%eax");
	else {
		/*
		 * salva em arg[n] se for argumento
		 */
		sprintf(str,"%s","\n\t pushl %%eax");
		strcat(arg[pos],str);
	}
}
/*
 * este procedimento gera o codigo para o resgistro de ativacao de
 * um procedimento ou funcao
 */
void cod_inicfunc(char *nome){
	fprintf(arq,"\n\t .global %s",nome);
	fprintf(arq,"\n\t .type %s, @function",nome);
}
/*
 * este procedimento gera o codigo para o rotulo do programa inicial
 * e dos procedimentos e funcoes
 */
void cod_rotulofunc(char *nome){
	fprintf(arq,"\n_%s:",nome);
}

/*
 * este procedimento gera o codigo para o retorno de funcao
 */
void cod_ret(void){
	fprintf(arq,"\n\t movl %%ebp,%%esp");
	fprintf(arq,"\n\t popl %%ebp");
	fprintf(arq,"\n\t ret");
}

/*
 * este procedimento gera o codigo para o registro de ativacao
 * de um procedimento ou de uma funcao
 */
void cod_regati(void){
	fprintf(arq,"\n\t pushl %%ebp");
	fprintf(arq,"\n\t movl %%esp,%%ebp");
}

/*
 * este procedimento gera o codigo em linguagem assembly para a atribuicao
 */
void l_val(char *nome,int off,int l){
	fprintf(arq,"\n\t popl %%eax");
	if(l == 0) fprintf(arq,"\n\t movl %%eax,%s",nome);
	else fprintf(arq,"\n\t movl %%eax,-%d(%%ebp)",off);
}

/*
 * este procedimento gera o codigo para a chamada de uma funcao ou procedimento
 */
void call_func(char *nome){
	fprintf(arq,"\n\t call %s",nome);
}

/*
 * este procedimento gera o codigo em linguagem assembly para comparacao
 * entre as clausulas do comando CASE
 */
void comp_case(int look){
	if(look==ID || look==CADEIA){
		fprintf(arq,"\n\t popl %%edi");
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t scasl");
		fprintf(arq,"\n\t pushl %%eax");
	}
	else {
		fprintf(arq,"\n\t popl %%edx");
		fprintf(arq,"\n\t popl %%eax");
		fprintf(arq,"\n\t cmpl %%eax,%%edx");
		fprintf(arq,"\n\t pushl %%eax");
	}
}

/*
 * este procedimento gera o codigo em linguagem de montagem para o fim do programa Pascal
*/
 void termina(void){
 	fprintf(arq,"\n\t int $128");
	fprintf(arq,"\n\t movl &1,%%eax");
	fprintf(arq,"\n\t movl &0,%%ebx");
	fprintf(arq,"\n\t int &128");
 }
 /*
  * este procedimento gera o codigo em linguagem de montagem para a verificacao da repeticao do for
*/
void termina_for(char *nome){
	fprintf(arq,"\n\t popl %%ecx");
	fprintf(arq,"\n\t leal %s, %%eax",nome);
	fprintf(arq,"\n\t incl %%eax");
	fprintf(arq,"\n\t movl %%eax, %s",nome);
	fprintf(arq,"\n\t subl %%ecx, %%eax");
	fprintf(arq,"\n\t pushl %%ecx");
}

/*
 *gera o codigo para o rotulo de section .bss
*/
void cod_bss(void){
	fprintf(arq,"\n\t .section .bss");
}

/*
 * gera o codigo para o rotulo de section .rodata
*/
void cod_rodata(void){
	fprintf(arq,"\n\t .section .rodata");
}
/*
 * gera o rotulo para as constantes
*/
void rotulo_ct(char *nome){
	fprintf(arq,"\n%s:",nome);
}

/*
 * gera o codigo para as definicoes das constantes
*/
void define_ct(int i,char *nome,int num,float num2){
	if(i==ID) fprintf(arq," .asciz %s",nome);
	else if(i==NUM_I) fprintf(arq," .long %d",num);
	else fprintf(arq," .long %f",num2);
}
/*
 * gera o codigo para o inicio do programa em linguagem de montagem
*/
void inicio(char *str){
	fprintf(arq,"\t .section .text");
	fprintf(arq,"\n\t .global _%s",str);
	fprintf(arq,"\n\t jmp _%s",str);
}
