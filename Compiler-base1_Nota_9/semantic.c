#include "semantic.h"

FILE *outtmp, *out;
char *start, *last_pro, *variable;
int marcador = -1, next_case = -1;

/* protótipo de funções */
void encerra(int exit_code);
void leitura(int descriptor, char *address, int tamanho);
void escrita(int descriptor, char *address, int tamanho);
void abrir_arq(void);
void fechar_arq(void);
void atribuir(void);
void section_text(void);
void section_rodata(void);
void print_const_lex(void);
void print_const(void);
void section_bss(void);
void print_var(void);
void jmp_start(void);
void act_then(int counter_label_if);
void act_else(int counter_label_if, int END_L);
void put_label_if(int counter_label_if);
void mark(void);
void pop(const char *address);
void push(const char *address);
void multiplica(void);
void divide_float(void);
void divide_int(void);
void divide_resto(void);
void logic_and(void);
void plus(void);
void minus(void);
void logic_or(void);
void igual(void);
void diferente(void);
void menor_que(void);
void menor_igual_que(void);
void maior_igual_que(void);
void maior_que(void);
void salve_pro(void);
void call_pro(void);
void put_buffer_out(void);
void put_buffer_in(void);
void negar(void);
void expression_case(void);
void print_label_case(void);
void jmp_end_label_case(int l);
void put_end_label_case(int l);
void salve_variable(void);
void atribuir(void);
void inconditional_jmp(void);
void print_label(void);
void print_literal_statements(void);

/* funções */
void semantic(action_t action){
  switch(action){
  case EXIT:encerra(0);break;
  case MARK:mark();break;
  case OPEN:abrir_arq();break;
  case CLOSE:fechar_arq();break;
  case ATRIBUIR:atribuir();break;
  case SECTION_TEXT:section_text();break;
  case SECTION_RODATA:section_rodata();break;
  case PRINT_CONST_LEX:print_const_lex();break;
  case PRINT_CONST:print_const();break;
  case SECTION_BSS:section_bss();break;
  case PRINT_VAR:print_var();break;
  case JMP_START:jmp_start();break;
  case PUSH:push((*currentp).offset);break;
  case POP:pop((*currentp).offset);break;
  case POP2Acc:pop("%eax");break;
  case MULTIPLICA:multiplica();break;
  case DIVIDE_FLOAT:divide_float();break;
  case DIVIDE_INT:divide_int();break;
  case DIVIDE_RESTO:divide_resto();break;
  case LOGIC_AND:logic_and();break;
  case PLUS:plus();break;
  case MINUS:minus();break;
  case LOGIC_OR:logic_or();break;
  case IGUAL:igual();break;
  case DIFERENTE:diferente();break;
  case MENOR_QUE:menor_que();break;
  case MENOR_IGUAL_QUE:menor_igual_que();break;
  case MAIOR_IGUAL_QUE:maior_igual_que();break;
  case MAIOR_QUE:maior_que();break;
  case SALVE_PROC:salve_pro();break;
  case CALL_PRO:call_pro();break;
  case PUT_BUFFER_OUT:put_buffer_out();break;
  case PUT_BUFFER_IN:put_buffer_in();break;
  case NEGAR:negar();break;
  case EXPRESSION_CASE:expression_case();break;
  case INCONDITIONAL_JMP:inconditional_jmp();break;
  case PRINT_LABEL:print_label();break;
  case PRINT_LITERAL_STATEMENTS:print_literal_statements();break;
  case SALVE_VARIABLE: salve_variable();break;
  }
}

void encerra(int exit_code){
  fprintf(outtmp,"\n # finaliza o software\n");
  fprintf(outtmp,"\n\tmovl\t$1, %%eax\n");
  fprintf(outtmp,"\tmovl\t$%d, %%ebx\n", exit_code);	/* default 0 */
  fprintf(outtmp,"\tint\t$0x80\n");
}

void leitura(int descriptor, char *address, int tamanho){
  fprintf(outtmp,"\n # leitura\n");
  /* limpa a entrada antes da leitura */
  fprintf(outtmp,"\tmovl\t$0, buffer_in\t# limpa a entrada antes da leitura\n");
  /* chamando interrupção de leitura */
  fprintf(outtmp,"\tmovl\t$%d, %%ebx\n", descriptor);	/* default 0 */
  fprintf(outtmp,"\tleal\t%s, %%ecx\n", address);
  if(tamanho == -1){
	fprintf(outtmp,"\tmovl\t$255, %%edx\n");
  }
  else
    fprintf(outtmp,"\tmovl\t$%d, %%edx\n", tamanho);
  fprintf(outtmp,"\tmovl\t$3, %%eax\n");
  fprintf(outtmp,"\tint\t$0x80\n");
}

void escrita(int descriptor, char *address, int tamanho){
  fprintf(outtmp,"\n # escrita\n");
  /* chamando interrupção de escrita */
  fprintf(outtmp,"\tmovl\t$%d, %%ebx\n",descriptor);	/* default 1 */
  fprintf(outtmp,"\tleal\t%s, %%ecx\n", address);
  if(tamanho == -1){
    fprintf(outtmp,"\t# conta quantos caracteres há em buffer_out\n");
	fprintf(outtmp,"\tleal\tbuffer_out, %%edx\n");
	push("%edx");
	fprintf(outtmp,"\tcall\tstrlen\n");
	fprintf(outtmp,"\taddl\t$4, %%esp\n");
	fprintf(outtmp,"\tmovl\t%%eax, %%edx\n");
  }
  else
    fprintf(outtmp,"\tmovl\t$%d, %%edx\n", tamanho);
  fprintf(outtmp,"\tmovl\t$4, %%eax\n");
  fprintf(outtmp,"\tint\t$0x80\n");
  /* limpa a saída após a escrita */
  fprintf(outtmp,"\tmovl\t$0, buffer_out\t# limpa a saída após a escrita\n");
}

void abrir_arq(void){
}

void fechar_arq(void){
}

void section_text(void){
  start = strdup(lexema);
  fprintf(outtmp,"\n\t\t.section.text\n");
  fprintf(outtmp,"\t\t.global _%s\n", start);
  fprintf(outtmp,"\tjmp\t_%s\n", start);
}

void section_rodata(void){
  fprintf(outtmp,"\n\t\t.section.rodata\n");
  fprintf(outtmp,"\tstrTrue:\t.asciz \"TRUE\"\n");
  fprintf(outtmp,"\tstrFalse:\t.asciz \"FALSE\"\n");
  fprintf(outtmp,"\tconvFloatStr:\t.asciz \"\\\"%%g\\\"\"\n");
}

void print_const_lex(void){
  tupla_t *tp = gettupla(getlast());
  fprintf(outtmp,"\t%s:\t",(*tp).lexema);
}

void print_const(void){
  tupla_t *tp = gettupla(getlast());

  switch((*tp).type){
	case BOOL:
	  fprintf(outtmp,".byte %s # não sei se é double\n",lexema);
	  break;
	case INT:
	  fprintf(outtmp,".long %s\n",lexema);
	  break;
	case FLOAT:
	  fprintf(outtmp,".double %s # não sei se é double\n",lexema);
	  break;
	case CHARACTER:
	  fprintf(outtmp,".ascii %s\n",lexema);
	  break;
	case TEXT:
	  fprintf(outtmp,".asciz %s\n",lexema);
	}
}

void section_bss(void){
  fprintf(outtmp,"\n\t\t.section.bss\n");
  fprintf(outtmp,"\t.comon buffer_out,2048,4\n");
  fprintf(outtmp,"\t.comon buffer_in,2048,4\n");
}

void print_var(void){
  int i, fim = getlast();
  tupla_t *tp, *tp_tipo;

  tp_tipo =gettupla(findindex(lexema));
  for(i = marcador; i <= fim; i++){
	  tp = gettupla(i);
	  switch((*tp_tipo).type){
	    case BOOL:
		  (*tp).type = BOOL;
		  fprintf(outtmp,"\t.comon %s,1,4\t# boolean\n",(*tp).offset);
		  break;
		case INT:
		  (*tp).type = INT;
		  fprintf(outtmp,"\t.comon %s,4,4\t# integer\n",(*tp).offset);
		  break;
		case FLOAT:
		  (*tp).type = FLOAT;
	      fprintf(outtmp,"\t.comon %s,8,4\t# real\n",(*tp).offset);
	      break;
	    case CHARACTER:
		  (*tp).type = CHARACTER;
	      fprintf(outtmp,"\t.comon %s,1,4\t# char\n",(*tp).offset);
	      break;
	    case TEXT:
		  (*tp).type = TEXT;
	      fprintf(outtmp,"\t.comon %s,256,4\t# string\n",(*tp).offset);
         break;
       default:
        (*tp).type = (*tp_tipo).type;
        fprintf(outtmp,"\t.comon %s,256,4\t# %s\n",
        			(*tp).offset, (*tp_tipo).lexema);
	  }
  }
  marcador = -1;
}

/* begin */
void jmp_start(void){
  fprintf(outtmp,"\n _%s:\n",start);
}

/* if-then-else */
void act_then(int counter_label_if){
  /* faz a verificação para ver se o valor
   * retornado pela expressão é TRUE ou FALSE */
  fprintf(outtmp,"\tjz\t.L_IF_%d\n", counter_label_if);
}

void act_else(int counter_label_if, int END_L){
  fprintf(outtmp,"\tjmp\t.L_IF_%d\n", counter_label_if);
  fprintf(outtmp," .L_IF_%d:\n", END_L);
}

void put_label_if(int counter_label_if){
  fprintf(outtmp," .L_IF_%d:\n", counter_label_if);
}

/* pilha */
void push(const char *address){
  fprintf(outtmp,"\tpush\t%s\n", address);
}

void pop(const char *address){
  fprintf(outtmp,"\tpop\t%s\n", address);
}

/* expressão */
void multiplica(void){
  fprintf(outtmp,"\n\t# multiplica\n");
  pop("%eax");
  fprintf(outtmp,"\timul\t(%%esp)\n");
  fprintf(outtmp,"\taddl\t$4,%%esp\n");
  push("%eax");
}

void divide_float(void){ /* / */
  fprintf(outtmp,"\n\t# divide_float\n");
  pop("%edx");
  pop("%eax");
  push("%edx");
  fprintf(outtmp,"\txorl\t%%edx, %%edx\n");
  fprintf(outtmp,"\tidiv\t(%%esp)\n");
  fprintf(outtmp,"\taddl\t$4,%%esp\n");
  push("%eax");
}

void divide_int(void){ /* DIV */
  fprintf(outtmp,"\n\t# divide_int\n");
  pop("%edx");
  pop("%eax");
  push("%edx");
  fprintf(outtmp,"\txorl\t%%edx, %%edx\n");
  fprintf(outtmp,"\tidiv\t(%%esp)\n");
  fprintf(outtmp,"\taddl\t$4,%%esp\n");
  push("%eax");
}

void divide_resto(void){ /* MOD */
  fprintf(outtmp,"\n\t# resto\n");
  pop("%edx");
  pop("%eax");
  push("%edx");
  fprintf(outtmp,"\txorl\t%%edx, %%edx\n");
  fprintf(outtmp,"\tidiv\t(%%esp)\n");
  fprintf(outtmp,"\taddl\t$4,%%esp\n");
  push("%edx");
}

void logic_and(void){
	fprintf(outtmp,"\n\t# AND\n");
	pop("%edx");
	pop("%eax");
	fprintf(outtmp,"\tandl\t%%edx, %%eax\n");
	push("%eax");
}

void plus(void){
  fprintf(outtmp,"\n\t# plus\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\taddl\t%%edx, %%eax\n");
  push("%eax");
}

void minus(void){
  fprintf(outtmp,"\n\t# minus\n");
	pop("%edx");
	pop("%eax");
	fprintf(outtmp,"\tsbbl\t%%edx, %%eax\n");
	push("%eax");
}

void logic_or(void){
  fprintf(outtmp,"\n\t# OR\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\torl\t%%edx, %%eax\n");
  push("%eax");
}

void igual(void){ /* comparação */
  fprintf(outtmp,"\n\t# igualdade\n");
  fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\tcmpl\t%%eax,%%edx\n");
  /* salva o resultado na pilha */
  fprintf(outtmp,"\tsetz\t%%bl\n");
  push("%ebx");
}

void diferente(void){
  fprintf(outtmp,"\n\t# diferença\n");
  fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\tcmpl\t%%eax,%%edx\n");
  /* salva o resultado na pilha */
  fprintf(outtmp,"\tsete\t%%bl\n");
  push("%ebx");
}

void menor_que(void){
    fprintf(outtmp,"\n\t# less than\n");
	fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
	pop("%edx");
	pop("%eax");
	fprintf(outtmp,"\tcmpl\t%%eax,%%edx\n");
	/* salva o resultado na pilha */
	fprintf(outtmp,"\tsetl\t%%bl\n");
	push("%ebx");
}

void menor_igual_que(void){
  fprintf(outtmp,"\n\t# less equal than\n");
	fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
	pop("%edx");
	pop("%eax");
	fprintf(outtmp,"\tcmpl\t%%eax,%%edx\n");
	/* salva o resultado na pilha */
	fprintf(outtmp,"\tsetle\t%%bl\n");
	push("%ebx");
}

void maior_igual_que(void){
  fprintf(outtmp,"\n\t# great equal than\n");
  fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\tcmpl\t%%eax,%%edx\n");
  /* salva o resultado na pilha */
  fprintf(outtmp,"\tsetge\t%%bl\n");
  push("%ebx");
}

void maior_que(void){
  fprintf(outtmp,"\n\t# great than\n");
  fprintf(outtmp,"\txorl\t%%ebx, %%ebx\n");
  pop("%edx");
  pop("%eax");
  fprintf(outtmp,"\tcmpl\t%%eax, %%edx\n");
  /* salva o resultado na pilha */
  fprintf(outtmp,"\tsetg\t%%bl\n");
  push("%ebx");
}

/* procedimentos */
void salve_pro(void){
	last_pro = strdup(lexema);
}

void call_pro(void){
	if(!strcmp(last_pro,"WRITE"))
		escrita(1, "buffer_out", -1);
	else if(!strcmp(last_pro,"READ"))
		leitura(0, "buffer_in", -1);
	else
		fprintf(outtmp,"\tcall\t%s\n",last_pro);
}

/* esta função joga quaquer informação no buffer_out */
void put_buffer_out(void){
	static int count_bool = 0, lout = 0, pout = 0;
	tupla_t *tp = gettupla(findindex(lexema));
/*
 * 1. identificar o tipo
 * 2. se for string ou caracter basta associar no final de buffer_out
 * 3. se for booleano salvar TRUE ou FALSE
 * 4. se for número converter para string e concatenar no final
 */
   fprintf(outtmp,"\n\t# coloca a informação no buffer de saída\n");
	switch((*tp).type){
	case BOOL:
   		fprintf(outtmp,"\t# boolean\n");
		fprintf(outtmp,"\tcmpl\t$0, %s\n",(*tp).offset);
		fprintf(outtmp,"\tjz\tfalse%d\n", count_bool++);
		/* copia o conteúdo de strTrue para buffer_out */
		fprintf(outtmp,"\tleal\tstrTrue, %%esi\n");
		fprintf(outtmp,"\tjmp fimBool%d\n", count_bool++);
		fprintf(outtmp," false%d:\n", count_bool - 2);
		/* copia o conteúdo de strFalse para buffer_out */
		fprintf(outtmp,"\tleal\tstrFalse, %%esi\n");
		fprintf(outtmp," fimBool%d:\n", count_bool - 1);
		fprintf(outtmp,"\tleal\tbuffer_out, %%edi\n");
		fprintf(outtmp,"\tcld\n");
   		fprintf(outtmp," .LOUT%d:",lout);
   		fprintf(outtmp,"\tcmpb\t$0, (%%esi)\n");
   		fprintf(outtmp,"\tmovsb\n");
   		fprintf(outtmp,"\tjnz\t.LOUT%d\n",lout++);
   		break;
	case INT:
   		/* itoa */
		fprintf(outtmp,"\t#itoa");
		fprintf(outtmp,"\tmovl\t%s, %%eax\n", (*tp).offset);
		fprintf(outtmp,"\tcmpl\t$0, %%eax\n");
		fprintf(outtmp,"\tjge\t.POUT%d\n", pout);
		fprintf(outtmp,"\tmovb\t$45, (%%esi)\n");
		fprintf(outtmp,"\tincl\t%%esi\n");
		fprintf(outtmp,"\tnegl\t%%eax\n");
		fprintf(outtmp," .POUT%d:\n",pout++);
		fprintf(outtmp,"\tmovl\t$10, %%ebx\n");
		fprintf(outtmp,"\txorl\t%%ecx, %%ecx\n");
		fprintf(outtmp,"\tpushl\t%%ecx\n");
		fprintf(outtmp,"\tincl\t%%ecx\n");
		fprintf(outtmp," .LOUT%d:\n",lout);
		fprintf(outtmp,"\txorl\t%%edx, %%edx\n");
		fprintf(outtmp,"\tdivl\t%%ebx\n");
		fprintf(outtmp,"\taddb\t$48, %%dl\n");
		fprintf(outtmp,"\tpushl\t%%edx\n");
		fprintf(outtmp,"\tincl\t%%ecx\n");
		fprintf(outtmp,"\torl\t%%eax, %%eax\n");
		fprintf(outtmp,"\tjnz\t.LOUT%d\n",lout++);
		fprintf(outtmp,"\tleal\tbuffer_out, %%esi\n");
		fprintf(outtmp," .LOUT%d:\n",lout);
		fprintf(outtmp,"\tpopl\t%%edx\n");
		fprintf(outtmp,"\tmovb\t%%dl, (%%esi)\n");
		fprintf(outtmp,"\tincl\t%%esi\n");
		fprintf(outtmp,"\tloop\t.LOUT%d\n",lout++);
		break;
	case FLOAT:
   		fprintf(outtmp,"\t# float\n");
		fprintf(outtmp,"\tpush\t%s\n",(*tp).offset);
		fprintf(outtmp,"\tpush\t$convFloatStr\n");
		fprintf(outtmp,"\tpush\t$buffer_out\n");
		fprintf(outtmp,"\tcall\tsprintf\n");
		fprintf(outtmp,"\taddl\t$4, %%esp\n");
		break;
	case CHARACTER:
   		fprintf(outtmp,"\t# char\n");
		fprintf(outtmp,"\tleal\tbuffer_out, %%edi\n");
		fprintf(outtmp,"\txorl\t%%eax, %%eax\n");
		fprintf(outtmp,"\trepne scasb\n");
		fprintf(outtmp,"\tmovb\t%s, %%edi\n", (*tp).offset);
		fprintf(outtmp,"\tincl\t%%edi\n");
		fprintf(outtmp,"\tmovb\t$0, %%edi");
	    break;
	case TEXT:
    	fprintf(outtmp,"\t# string\n");
		fprintf(outtmp,"\tleal\tbuffer_out, %%edi\n");
		fprintf(outtmp,"\txorl\t%%eax, %%eax\n");
		fprintf(outtmp,"\trepne scasb\n");
		fprintf(outtmp,"\tleal\t%s, %%esi\n", (*tp).offset);
		fprintf(outtmp," .LOUT%d:\n",lout);
		fprintf(outtmp,"\tcmpb\t$0, (%%esi)\n");
		fprintf(outtmp,"\tmovsb");
		fprintf(outtmp,"\tjnz\t.LOUT%d\n",lout++);
		break;
	case NOTYPE:
		;
	}
}

void put_buffer_in(void){
	fprintf(outtmp,"\n\t# converte os dados lidos para ...\n");
	switch((*currentp).type){
	case BOOL:
   		fprintf(outtmp,"\t# boolean\n");
		fprintf(outtmp,"\tpushl\t$buffer_in\n");
		fprintf(outtmp,"\tcall\tatoi\n");
		fprintf(outtmp,"\tmovl\t%%eax, %s\n",(*currentp).offset);
		fprintf(outtmp,"\taddl\t$4, %%esp\n");
   		break;
	case INT:
   	/* atoi */
		fprintf(outtmp,"\t# atoi\n");
		fprintf(outtmp,"\tpushl\t$buffer_in\n");
		fprintf(outtmp,"\tcall\tatoi\n");
		fprintf(outtmp,"\tmovl\t%%eax, %s\n",(*currentp).offset);
		fprintf(outtmp,"\taddl\t$4, %%esp\n");
		break;
	case FLOAT:
		fprintf(outtmp,"\t# float\n");
   		fprintf(outtmp,"\tpushl\t$buffer_in\n");
		fprintf(outtmp,"\tcall\tatof\n");
		fprintf(outtmp,"\tmov\t%%eax, %s\n",(*currentp).offset);
		fprintf(outtmp,"\taddl\t$4, %%esp\n");
		break;
	case CHARACTER:
   		fprintf(outtmp,"\t# char\n");
		fprintf(outtmp,"\tleal\tbuffer_in, %%esi\n");
		fprintf(outtmp,"\tleal\t%s, %%edi\n", (*currentp).offset);
		fprintf(outtmp,"\tmovsb\n");
	    break;
	case TEXT:
    	fprintf(outtmp,"\t# string\n");
		fprintf(outtmp,"\tleal\tbuffer_in, %%esi\n");
		fprintf(outtmp,"\tpushl\t%%esi\n");
		fprintf(outtmp,"\tleal\t%s, %%edi",(*currentp).offset);
		fprintf(outtmp,"\tpushl\t%%edi\n");
		fprintf(outtmp,"\tcall\tstrcpy\n");
		fprintf(outtmp,"\taddl\t$8, %%esp\n");
	   break;
	case NOTYPE:
		;
	}
}

void negar(void){
	pop("%eax");
	fprintf(outtmp,"not\t%%eax\n");
	push("%eax");
}

void oposto(void){
	pop("%eax");
	fprintf(outtmp,"neg\t%%eax\n");
	push("%eax");
}


/* case */
void expression_case(void){
	static int prox = 0, end = 0, end1 = 0, end2 = 0;
   switch((*currentp).type){
   case BOOL:
 	   fprintf(outtmp,"\tmovb\t%s, %%edx\n", (*currentp).offset);
   	fprintf(outtmp,"\tcmpb\t%%al, %%dl\n");
      break;
   case INT:
   case FLOAT:
    fprintf(outtmp,"\tmovl\t%s, %%edx\n", (*currentp).offset);
  	fprintf(outtmp,"\tcmpl\t%%eax, %%edx\n");
   	break;
   case CHARACTER:
	   fprintf(outtmp,"\tmovb\t%s, %%edx\n", (*currentp).offset);
   	fprintf(outtmp,"\tcmpb\t%%al, %%dl\n");
      break;
   case TEXT:
      fprintf(outtmp,"\tleal\t%s, %%edi\n", (*currentp).offset);
      fprintf(outtmp,"\tmovl\t%%eax, %%esi\n");
      fprintf(outtmp,"\tcld\n");
      fprintf(outtmp," .PROX%d:\n",prox);
      fprintf(outtmp,"\tcmpb\t$0, (%esi)\n");
      fprintf(outtmp,"\tje\t.ENDS1_%d\n",end1);
      fprintf(outtmp,"\tcmpb\t$0, (%edi)\n");
      fprintf(outtmp,"\tje\t.ENDS2_%d\n",end2);
      fprintf(outtmp,"\tcmpsb\n");
      fprintf(outtmp,"\tje\t.PROX%d\n",prox++);
      fprintf(outtmp,"\tdecl\t%%esi\n");
      fprintf(outtmp,"\tdecl\t%%edi\n");
      fprintf(outtmp,"\tmovb\t(%%esi), %%al\n");
      fprintf(outtmp,"\tcmpb\t(%%edi), %%al\n");
      fprintf(outtmp,"\tjg\t.ENDS2_%d\n",end2);
      fprintf(outtmp,"\t .ENDS1_%d:\n",end1++);
      fprintf(outtmp,"\txorl\t%%eax, %%eax\n");
      fprintf(outtmp,"\tcmpb\t$0, (%%edi)\n");
      fprintf(outtmp,"\tje\t.ENDS%d\n",end);
      fprintf(outtmp,"\tdecl\t%%eax\n");
      fprintf(outtmp,"\tjmp\t.ENDS%d\n",end);
      fprintf(outtmp," .ENDS2_%d:\n",end2++);
      fprintf(outtmp,"\tmovl\t$1, %%eax\n");
      fprintf(outtmp," .ENDS%d\n",end++);
   }
}

void print_conditional_jmp_label_case(int l){
   fprintf(outtmp,"\tjz\t.L_CASE_%d\n",l);
}

void jmp_end_label_case(int l){
	fprintf(outtmp,"\tjmp\t.L_END_CASE_%d\n",l);
}

void print_next_label_case(int l){
	fprintf(outtmp," .L_CASE_%d:\n",l);
}

void put_end_label_case(int l){
	fprintf(outtmp," .L_END_CASE_%d:\n",l);
}

/* while */
void put_label_while(int l){
	fprintf(outtmp," .L_WHILE_%d:\n",l);
}

void jmp_when_false(int l){
	fprintf(outtmp,"\tjz\t.L_END_WHILE_%d:\n",l);
}

void jmp_to_begin(int l){
	fprintf(outtmp,"\tjmp\t.L_WHILE_%d:\n",l);
}

void put_end_label_while(int l){
	fprintf(outtmp," .L_END_WHILE_%d:\n",l);
}

/* repeat */
void print_label_repeat(int l){
	fprintf(outtmp," .L_REPEAT_%d:\n",l);
}

void jmp_if_false(int l){
	fprintf(outtmp,"\tjz\t.L_REPEAT_%d:\n",l);
}

/* atribuição */
void salve_variable(void){
	variable = strdup(lexema);
}
void atribuir(void){
	fprintf(outtmp,"\tmovl\t%%eax, %s\n", variable);
}
/* for */
void put_label_for(int l){
    fprintf(outtmp,"\t# prólogo do for %d\n",l);
    fprintf(outtmp, " .L_FOR_%d:\n", l);
}

void verifica_contador(const char *s, int l){
    fprintf(outtmp,"\t# valia a contagem\n",l);
    fprintf(outtmp,"\tmovl\t%s, %%edx\n",s);
    fprintf(outtmp,"\tcmpl\t%%eax, %%edx\n");
    fprintf(outtmp,"\tja\t.L_END_FOR_%d\n",l);
}

void jmp_label_for(const char *s, int l){
	fprintf(outtmp,"\t# epílogo do for %d\n",l);
	fprintf(outtmp,"\tinc\t%s\n",s);
    fprintf(outtmp,"\tjmp\t.L_FOR_%d\n",l);
    fprintf(outtmp," .L_END_FOR_%d:\n",l);
}

/* goto */
void inconditional_jmp(void){
	fprintf(outtmp,"\tjmp\t%s\n",(*currentp).offset);
}

void print_label(void){
	fprintf(outtmp," %s:\n",(*currentp).offset);
}

void print_literal_statements(void){
   int size = getlast() + 1, i;
   tupla_t *tp;
   char buf[1024];
   /* procura a seção rodata ou bss */
   do{
	   if(fgets(buf, sizeof buf, outtmp) == NULL){
	   	if (ferror(outtmp)) {
   	   	perror("Erro no arquvo de saída");
      	  	fclose(outtmp);
	        	exit(1);
   	 	}
         return;
      }
      fputs(buf,out);
   }while(!strstr(buf, ".rodata"));
   /* insere os dados */
   for(i = 0; i < size; i++){
   	tp = gettupla(i);
      if((*tp).qual == LITERAL)
      	fprintf(out,"\t%s:\t.asciz %s\n",(*tp).offset, (*tp).lexema);
   }
   while(fgets(buf, sizeof buf, outtmp))
   	fputs(buf,out);
}

void mark(void){
  if(marcador == -1)
	marcador = getlast();
}