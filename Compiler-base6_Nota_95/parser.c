#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexan.h"
#include "parser.h"
#include "symbol.h"
#include "pilha.h"
#include "codificador.h"
/*
 * Definicao do simbolo vazio
*/
#define EPSILON ;

int lexentry;

/*
 * t_bottom aponta para a proxima posicao da tabela de simbolos
*/
int t_bottom;

/*
 * endstream aponta para a proxima posicao livre de lexstream
*/
int endstream;

/*
 * reg_ati indica o inicio de um registro de ativacao de
 * um procedimento ou funcao
*/
int reg_ati;

/*
 * reg_lexstream indica a posicao apartir da qual foram armazenadas as variaveis
 * de uma funcao ou procedimento
*/
int reg_lexstream;

/*
 * reg_offset sera utilizada para indicar a posicao a partir da qual foram armazenados
 * os offsets para as variaveis
 */
int reg_offset;

/*
 * buffer  e o vetor onde estara o programa a ser compilado
*/
FILE *buffer,*arq;

/*
 * lexeme sera o vetor onde serao salvos os identificadores, os
 * operadores logicos(and,or,etc), os operadores relacionais (<,<>,etc)
*/
char lexeme[256];

/*
 * t
 * linha guarda a linha do programa que esta sendo compilada
*/
int linha =0;
/*
 * serao utilizadas para impedir a dupla declaracao de types, const e var
*/
int t=0,c=0,v=0;

/*
 * variavel tip,t t1,t2,t3,t4 serao utilizadas para verificacao de tipos
*/
int tip,t,t1,t2,t3,t4;
/*
 * conta quantas variaveis existem para cada tipo
 */
int marcatipo;

/*
 *L será utilizada para a geração de rótulos durante o programa
 */
int L=1;
/*
 * salva o nome do programa para ser utiliado na inicializacao
 */
char nome_prog[256];
char nome[256];
/*
 * sera utilizado para salvar as expressoes que serao parametros de um procedimento ou funcao
 */
char arg[16][256];

/*
 * variavel utilizada para guardar os numeros inteiros
 */
int num1;
/*
 * level indica  se o escopo eh do programa principal ou o escopo de um procedimento
 * ou funcao
 */
int level;

/*
 * par serao utilizados para a realizacao das operacoes sobre os
 * parametros de um procedimento ou de uma funcao
 */
int par;
/*
 * sera utilizado para indicar a posicao em arg[][] onde estarao salvo os comandos
 * relativos a resolucao das expressoes(se existirem) que compoem os argumentos
 * de uma chamada de procedimento ou funcao
 */
int pos;
/*
 * indica se a resolucao e de expressoes para parametros
 */
int param=0;
int ct=0;

/*
 * este procedimento busca o tipo da variavel que esta armazenada na tabela de
 * simbolos
 */
int busca_tipo(int t){
	switch(t){
		case CH:
			return 1;
			break;
		case BLN:
			return 1;
			break;
		case INT:
			return 4;
			break;
		case RL:
			return 8;
			break;
		default:
			return 0;
	}
}

/*
 * este procedimento insere os tipos na tabela de simbolos para as variaveis declaradas
 */
void puttipo(int tipo){
	int i,aux,tip1=0;
	int set;

	tip1=busca_tipo(tipo);

	set = marcatipo*tip1;
	aux = t_bottom - 1;
	for(i=0; i < marcatipo; i++){
		table[aux].type=tipo;
		table[aux].offset = set;
		table[aux].nivel=level;
		set = set - tip1;
		if(level == 0)fprintf(arq,"\n.commom %s,%d,4,",table[aux].pos,tip1);
		aux--;
	}
}

/*
 * coloca o offset para os parametros de um procedimento ou de uma funcao
 */
void putoffset(int t,int tpe){
	int aux,set,i;

	set = 4+t*4;
	aux=t_bottom-1;
	for(i=0; i<t; i++){
		table[aux].offset=set;
		table[aux].type=tpe;
		table[aux].nivel=level;
		set = set-4;
		aux--;
	}
}

/*
 * Verifica fim de comando
*/
void appendix(void){
	if (lookahead == ';'){
		match(';');

		/*
		 * Verifica se existe um ELSE apos um ponto-e-virgula
		 * Se existir acusa erro, ja que nao existe ponto-virgula
		 * apos um comando ou um bloco de comandos existentes em um THEN
		 */
		if (lookahead == ELSE) {
			printf("\n Linha %d: Sobrando ponto-virgula",linha);
			exit(2);
		}
		comando();
		appendix();
	}
	else if (lookahead == PT){
		match(PT);
		/*
		 * gera o codigo em linguagem de montagem para o fim do programa Pascal
		 */
		termina();
	}
}


/*
 * Verifica a sintaxe dos Fatores
*/
void F(void){

	if(lookahead == 0){
		printf("\n Linha %d: Variavel nao declarada",linha);
		exit(2);
	}
	switch(lookahead){
		/*
		 * Verifica se o fator e uma das funcoes de biblioteca
		*/
		case ABS:
			match(lookahead);
			match('(');
			E();
			t4=t3;
			t4=verifica_tipo(t4,3);
			if(t4==err_tipo){
				printf("\n ERRO linha %d: Argumento de ABS tem que ser inteiro!",linha);
				exit(2);
			}
		        else{
				while(lookahead == ','){
					match(',');
					E();
				}
				match(')');
			}
			break;
		/*
		 * verifica se fator e uma expressao entre parenteses
		*/
		case '(':
			match('(');
			E();
			match(')');
			break;
		/*
		 * verifica se o fator e um fator negado
		*/
		case NOT:
			match(NOT);
			F();
			cod_not();
		break;
		default:
			constante();
	}
}

/*
 * Verifica a sintaxe dos termos
*/
void T(void){
	double x,y;
	token_t aux;
	/*
	 * chama a funcao Fator
	*/
	F();
	t1=t;
	while(lookahead == OP_MUL || lookahead == OP_DIV || lookahead == DIV || lookahead == MOD || lookahead == AND){
		switch(lookahead){
			case OP_MUL:
				match(lookahead);
				F();
				t2=t;
				t1=verifica_tipo(t1,t2);
				if(t1==err_tipo){
					printf("\n ERRO linha %d: Imcompatibilidade de tipos!",linha);
					exit(2);
				}
				x = desempilha();
				y = desempilha();
				empilha(x * y);
				cod_multiplicacao();
				empilha_acc();
				break;
			case OP_DIV:
				match(lookahead);
				F();
				t2=t;
				t1 = verifica_tipo(t1,t2);
				if(t1==err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
				x = desempilha();
			        y = desempilha();
				if(x==0){
					printf("\n Divisao por zero!");
					empilha(0);
				}
			        empilha(y/x);
				cod_divisao();
				empilha_acc();
				break;
			case DIV:
		        case MOD:
				aux=lookahead;
				match(lookahead);
				F();
				t2=t;
				t1=verifica_tipo(t1,t2);
				if(t1==err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
				cod_mod_div(aux);
				break;
			case AND:
			       match(AND);
			       F();
			       t2=t;
			       t1=verifica_tipo(t1,t2);
			       if(t1==err_tipo){
				       printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
				       exit(2);
			       }
			       /*
				* gera codigo para o comando AND
				*/
			       cod_and_or(AND);
		       	       break;
		}
	}
}

/*
 * Verifica a sintaxe de expressoes
*/
void E(void){
	double x,y;
	token_t aux;
	/*
	 * chama a funcao Termo
	*/
	T();
	t3=t1;
	while (lookahead == OP_ADD || lookahead == OP_SUB || lookahead == OR){
		switch(lookahead){
			case OP_ADD:
				match(OP_ADD);
				T();
				t4=t1;
				t3=verifica_tipo(t3,t4);
				if(t3 == err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
			        x = desempilha();
				y = desempilha();
				empilha(x + y);
				cod_soma(OP_ADD);
				empilha_acc();
				break;
			case OP_SUB:
				match(OP_SUB);
				T();
				t4=t1;
				t3=verifica_tipo(t3,t4);
				if(t3==err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
				x = desempilha();
				y = desempilha();
				empilha(y - x);
				cod_soma(OP_SUB);
				empilha_acc();
				break;
			case OR:
				match(OR);
				T();
				t4=t1;
				t3=verifica_tipo(t3,t4);
				if(t3==err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
				cod_and_or(OR);
				break;
		}
	}
	switch(lookahead){
		case OP_DIF:
		case OP_LE:
		case OP_LT:
		case OP_ME:
		case OP_MT:
		case OP_EQ:
			aux = lookahead;
			match(lookahead);
			T();
			t4=t1;
			t3=verifica_tipo(t3,t4);
			if(t3==err_tipo){
				printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
				exit(2);
			}
			operador_relacional(aux);
			while (lookahead == OP_ADD || lookahead == OP_SUB || lookahead == OR){
				match(lookahead);
				T();
				t4=t1;
				t3=verifica_tipo(t3,t4);
				if(t3==err_tipo){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
			}
		break;
		default:
			EPSILON;
	}
}

/*
 * Verifica a sintaxe dos comandos em Pascal
*/

void comando(void){
	int L_end = L, L_init = L,next_label;
        int ent_table,r,i;
	float res;
	token_t aux,look;

	/*
	 * Verifica se um identificador nao foi declarado
	*/
	if(lookahead == 0){
		printf("\n Linha %d: Varivel nao declarada",linha);
		exit(2);
	}
	switch(lookahead){
		/*
		 * verifica a sintaxe para as rotinas (read,write,etc)
		*/
		case READ:
		case WRITE:
			aux = lookahead;
			match(lookahead);
			match('(');
			look = lookahead;
			E();
			if (look == NUM_I) {				
				leitura_escrita(aux,&num1,4);
			}
			else if(look == NUM_R){
				leitura_escrita(aux,&lexnum,8);
			}
		        else if (look == ID){
				if(t==0){
					printf("\n ERRO linha %d: Tipo invalido!",linha);
					exit(2);
				}
				else
					leitura_escrita(aux,lexeme,t);
			}
			while(lookahead == ','){
				match(',');
				E();
				look = lookahead;
				if (look == NUM_I){
					leitura_escrita(aux,&num1,4);
				}
				else if(look == NUM_R)
					leitura_escrita(aux,&lexnum,8);
				else if (look == ID){
					if(t==0){
						printf("\n ERRO linha %d: Tipo invalido!",linha);
						exit(2);
					}
					else leitura_escrita(aux,lexeme,t);
				}
			}
			match(')');
			break;
		case ID:
			ent_table = lexentry;
			strcpy(nome,lexeme);
			r=table[ent_table].type;
			match(ID);
			if (lookahead == OP_ASSIG) {
				match(OP_ASSIG);
				E();
				if((r != t3) && (t3 > r) ){
					printf("\n ERRO linha %d: Incompatibilidade de tipos!",linha);
					exit(2);
				}
				res=desempilha();
				table[ent_table].valor=res;
				/*
				 * l_val atribui o valor calculado a correspondente variavel
				 */
				l_val(nome,table[busca(nome)].offset,level);
			}
			/*
			 * verifica se eh a chamada de uma funcao ou procedimento
			 */
			else if(lookahead == '('){
				match('(');
				pos=0;
				param=1;
				E();
				pos++;
				while(lookahead == ','){
					match(',');
					E();
					pos++;
				}
				match(')');
				match(';');
				for(i = pos;i >= 0; i--)
					fprintf(arq,"%s",arg[i]);
				call_func(nome);
				param=0;
			}
			/*
			 * se nao for atribuicao
			 */
			else {
				empilha(table[ent_table].valor);
				cod_empilha(ID);
				E();
			}
		break;
		case BEGIN:
			/*
			 * Chama a funcao block() para verificar a sintaxe
			 * dos comandos de um bloco
			*/
			match(BEGIN);
			comando();
			appendix();
			match(END);
			appendix();
		break;
		case IF:
			match(IF);
			E();
			match(THEN);
			emitGofalse(L++);
			comando();
			appendix();
			if(lookahead == ELSE) {
				emitGoto(L);
				emitLabel(L_end);
				L_end=L;
				L++;
				match(ELSE);
				if(lookahead == BEGIN){
				       	comando();
					appendix();
				}
				else comando();
			}
			emitLabel(L_end);
		break;
		case CASE:
			match(CASE);
			E();
			match(OF);
			look=lookahead;
			do{
				constante();
				comp_case(look);
				emitGotrue(++L);
				while(lookahead == ','){
					match(',');
					constante();
					comp_case(look);
					emitGotrue(L);
				}
				next_label=L;
				match(PONTO);
				if(lookahead == BEGIN){
					comando();
					appendix();
				}
				else {
					comando();
					match(';');
				}
				emitGoto(L_end);
				emitLabel(next_label);
		       }while(lookahead != END);
		       match(END);
		       emitLabel(L_end);
		       appendix();
		break;
		case WHILE:
			emitLabel(L);
			match(WHILE);
			E();
			match(DO);
			L_end = ++L;
			emitGofalse(L_end);
			comando();
			emitGoto(L_init);
			emitLabel(L_end);
			L++;
		break;
		case REPEAT:
			match(REPEAT);
			emitLabel(L);
			L_init = L;
			L++;
			comando();
			appendix();
			match(UNTIL);
			E();
			emitGofalse(L_init);
			appendix();
		break;
		case FOR:
			match(FOR);
			if(lookahead == 0){
				printf("\n Linha %d: Variavel nao declarada",linha);
				exit(2);
			}
			strcpy(nome,lexeme);
			match(ID);
			match(OP_ASSIG);
			E();
			l_val(nome,0,table[busca(nome)].nivel);
			match(TO);
			E();
			match(DO);
			emitLabel(L);
			L_init = L;
 			L++;
			comando();
			termina_for(nome);
			emitGotrue(L_init);
		break;
		default:
			EPSILON;
	}
}

/*
 * Verifica a sintaxe da declacao do corpo de um programa Pascal
*/
void block(void){
	cod_rotulofunc(nome_prog);
	match(BEGIN);
	comando();
	appendix();
	match(END);
	appendix();	
}


/*
 * Verifica a sintaxe da declaracao dos parametros, se houverem
*/
void parlist(void){
	int n=0;

	/*
	 * Salva o inicio do registro de ativacao para apagar os
	 * registros das variaveis locais da tabela de simbolos
	*/
	reg_ati = t_bottom-1;
	reg_lexstream = endstream;

	if(lookahead == VAR){
		match(VAR);
		match(ID);
		n++;
		while(lookahead == ',') {
			match(',');
			match(ID);
			n++;
		}
		match(PONTO);
		tiposimples();
		putoffset(n,tip);
		if (lookahead == ';'){
			match(';');
			parlist();
		}
		else EPSILON;
	}
	else {
		match(ID);
		n++;
		while(lookahead == ',') {
			match(',');
			match(ID);
			n++;
		}
		match(PONTO);
		tiposimples();
		putoffset(n,tip);
		if (lookahead == ';'){
			match(';');
			parlist();
		}
		else EPSILON;
	}
}

/*
 * Verifica a sintaxe da declaracao da lista de parametros
 * de um procedimento ou de uma funcao
*/
void argdecl(void){
	if (lookahead == '(') {
		match('(');
		parlist();
		match(')');
	}
	else EPSILON;
}

/*
 * Verifica a sintaxe da declaracao de procedimentos e funcoes
*/
void methods(void){
	v=0;
	char nome[20];
	switch(lookahead){
		case FUNCTION:
			level=1;
			match(FUNCTION);
			strcpy(nome,lexeme);
			cod_inicfunc(lexeme);
			match(ID);
			argdecl();
			match(PONTO);
			tiposimples();
			match(';');
			header();
			cod_rotulofunc(nome);
			/*
			 * gera o codigo para o registro de ativacao
			 * da funcao
			 */
			cod_regati();
			match(BEGIN);
			comando();
			appendix();
			match(END);
			cod_ret();
			match(';');
			/*
			 * Retira as variaveis locais a funcao
			 * da tabela de simbolos
			*/
			level=0;
			t_bottom = reg_ati;
			endstream = reg_lexstream;
		break;
		case PROCEDURE:
			level=1;
			match(PROCEDURE);
			strcpy(nome,lexeme);
			cod_inicfunc(lexeme);
			match(ID);
			argdecl();
			match(';');
			header();
			cod_rotulofunc(nome);
			/*
			 * gera o codigo para o registro e ativacao
			 * do procedimento
			 */
			cod_regati();
			match(BEGIN);
			comando();
			appendix();
			match(END);
			cod_ret();
			match(';');
			/*
			 * Retira as variaveis locais a funcao
			 * da tabela de simbolos
			*/
			level=0;
			t_bottom = reg_ati;
			endstream = reg_lexstream;
		break;
		default:
			EPSILON;
	}
}
/*
 * verifica a sintaxe de uma lista de identificadores de variaveis
*/
void declist(void){

	match(ID);
       	while(lookahead == ',') {
		marcatipo++;
		match(',');
		match(ID);
	}
	match(PONTO);
	tiposimples();
	puttipo(tip);
	if (lookahead == ';'){
		match(';');
	}
}

/*
 * verifica a sintaxe das constantes
*/
void constante(void){
			switch(lookahead){
				case ID:
						if(level==0) cod_empilha(ID);
						else {
							par=table[busca(lexeme)].offset;
							cod_empilha2(par);
						}
						t=table[busca(lexeme)].type;
						empilha(table[lexentry].valor);
					match(ID);
					break;
				case NUM_I:
					empilha(num1);
					if(ct ==0)cod_empilha(NUM_I);
					tip=INT;
					t=3;
					match(NUM_I);
					break;
				case NUM_R:
					empilha(lexnum);
					if(ct==0)cod_empilha(NUM_R);
				     	t=4;
					tip=RL;
					match(NUM_R);
					break;
				case CADEIA:
					cod_empilha(ID);
					match(CADEIA);
					break;
				default:
					match(OP_ADD);
					if(lookahead == NUM_I || lookahead == NUM_R){
						match(lookahead);
					}
					else printf("\n ERRO linha %d: Constante invalida",linha);
			}
}
/*
 * Verifica a sintaxe dos tipos simples
*/
void tiposimples(void){
			switch(lookahead){
				case CHAR:
					match(CHAR);
					tip=CH;
					break;
				case INTEGER:
					match(INTEGER);
					tip=INT;
					break;
				case REAL:
					match(REAL);
					tip=RL;
					break;
				case BOOLEAN:
					match(BOOLEAN);
					tip=BLN;
					break;				
			}
}

/*
 *Verifica a sintaxe da declaracao de variaveis, de constantes e de tipos
*/
void tipodcl(void){
		int s=0;
		switch(lookahead){
			case VAR:
				if(v == 1){
					printf("\n ERRO linha %d: Declaracao invalida",linha);
					exit(2);
				}
			       	else {
					v=1;
					match(VAR);
					if(level==0)
					cod_bss();
					while(lookahead == ID){
						marcatipo=0;
						marcatipo++;
						declist();
					}
					tipodcl();
					}
					break;
			case CONST:
				s=0;ct=1;
				if(c==1) {
					printf("\n ERRO linha %d: Declaracao invalida",linha);
					exit(2);
				}
				else {
					c=1;
					cod_rodata();
					match(CONST);
					marcatipo = 0;
					while (lookahead == ID){
					   	marcatipo++;
						rotulo_ct(lexeme);
						match(ID);
						match(OP_EQ);
						if(lookahead == ID)
							define_ct(ID,lexeme,0,0);
						else if (lookahead == NUM_I)
							define_ct(NUM_I,"0",num1,0);
						else if (lookahead == NUM_R)
							define_ct(NUM_R,"0",0,lexnum);
						constante();
						match(';');
					}
					tipodcl();
				}
				ct=0;
				break;
			case TYPE:
				s=0;ct=1;
				if(t==1){
					printf("\n ERRO linha %d: Declaraca invalida!",linha);
					exit(2);
				}
				else{
					t=1;
					match(TYPE);
					marcatipo=0;
					while(lookahead ==ID){
						marcatipo++;
						match(ID);
						match(OP_EQ);						
						tiposimples();
						match(';');
					}
				 	tipodcl();
				}
				ct=0;
				break;
			default:
				EPSILON;
		}
}

/*
 * verifica a existencia de declaracoes de variaveis e funcoes e/ou procedimentos
 */
void header(void){
	/*
	 * verifica a declaracao de Types, das constantes e das variaveis globais
	 */
	tipodcl();
	/*
	 * verifica a sintaxe das funcoes e procedimentos
	 */
	while(lookahead == FUNCTION || lookahead == PROCEDURE)
	  		methods();

}

/*
 * Encontra o simbolo inicial de um programa Pascal (program)
 * verifica os comandos existentes no corpo do programa
 */
void trypascal(void){
	if(lookahead == PROGRAM){
		/*
		 * nivel lexico zero para o escopo do programa principal
		 */
		level=0;
		match(PROGRAM);
		lexentry = busca(lexeme);
		strcpy(nome_prog,lexeme);
		match(ID);
		inicio(nome_prog);
		if(lookahead == ';'){
			match(';');
			header();
			block();
		}
	}
	else{
		printf("\n ERRO linha %d: Identificador nao encontrado!",linha);
		exit(2);
	}
}
