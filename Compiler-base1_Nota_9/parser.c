#include "parser.h"

/* protótipos */
void bloco(void);
void comando(void);
void lista_parametros(void);
void expressao(action_t action);
void expressao_simples(action_t action);
void termo(action_t action);
void fator(action_t action);
boolean_t variavel(action_t action);
void tipo(action_t act);
void tipo_simples(action_t act);
void constante(action_t act);


/* esta função simplesmente verifica se o lexema corrente
 * corrente possui o token de idenfificador e o qualifica
 * em:
 * - UNDEFINED         (ainda não definido)
 * - RESERVED_WORD    (uma palavra reservada)
 * - ID_CON 	(uma constante)
 * - ID_TIP 	(um tipo, ou definido pelo usuário ou do próprio sistema)
 * - ID_VAR 	(uma variável)
 * - ID_PRO 	(um procedimento)
 * - ID_FUN 	(uma função)
 *   ID_LABEL 	(qualifica como um rótulo)
 * - CONSTANT	(uma constante numérica)
 * - LITERAL	(uma constante string)*/

void qualificador(int tok){
  tupla_t *tupla;
  if(lookahead == IDENTIFICADOR){
    /* a função gettupla acessa a tabela de símbolos
     * e tendo o lexema como chave retorna o ponteiro
     * para a tupla caso exista. Caso contrário retorna NULL*/
    tupla = (tupla_t *)gettupla(findindex(lexema));
    if(tupla)
      switch(tok){
      case LABEL:			(*tupla).qual = ID_LABEL;break;
      case CONST:     		(*tupla).qual = ID_CON;break;
      case TYPE:      		(*tupla).qual = ID_TIP;break;
      case VAR:       		(*tupla).qual = ID_VAR;break;
      case PROCEDURE: 		(*tupla).qual = ID_PRO;break;
      case FUNCTION:  		(*tupla).qual = ID_FUN;break;
      default:        		(*tupla).qual = UNDEFINED;
      }
  }
}


/* Esta função verifica se o lexema encontrado já está na
 * tabela de símbolos, se é um identificador e qual a qualificação
 * do identificador (UNDEFINED, ID_CON, ID_TIP, ID_VAR, ID_PRO,
 * ID_FUN, ID_LABEL).
 * Caso esteja tudo ok, returna 1, caso contrário um erro é emitido
 * e retornar 0.
 */
boolean_t match_qualifier(int qualifier, boolean_t showerro){
  boolean_t ok = FALSE;
  tupla_t *tupla;

  if(lookahead == IDENTIFICADOR){
    tupla = (tupla_t *)gettupla(findindex(lexema));
    if(tupla){
      ok = ((*tupla).qual == qualifier ? TRUE : FALSE);
      if(!ok && showerro)
			/* gera um erro avisando que o identificador
			 * não está de acordo com a qualificação
			 * esperada. */
			error(TYPE_INCOMPATIBLE);
    }
    else {
      /* gera um erro avisando que o símbolo ainda não foi
       * cadastrado na tabela de símbolos */
      if(showerro) error(UNKNOWN);
      ok = FALSE;
    }
  }
  return ok;
}

void programa(void){
  match(PROGRAM, NOACTION, EXP_PROGRAM);
  match(IDENTIFICADOR, SECTION_TEXT, EXP_ID);
  match(';', SECTION_RODATA, EXP_PTOVIRG);
  bloco();
  match('.', EXIT, EXP_PTO);
}

void bloco(void){
  int cont;
  tupla_t *tp;

  switch(lookahead){
  case LABEL:
  	match(LABEL, NOACTION, EXP_LABEL);
	qualificador(LABEL);
    match(IDENTIFICADOR, NOACTION, EXP_ID);
    while(lookahead == ','){
      match(',', NOACTION, EXP_VIRG);
      qualificador(LABEL);
      match(IDENTIFICADOR, NOACTION, EXP_ID);
    }
    match(';', NOACTION, EXP_PTOVIRG);
  case CONST:
    if(lookahead == CONST){
      match(CONST, NOACTION, EXP_CONST);
      do{
		qualificador(CONST);
		match(IDENTIFICADOR, PRINT_CONST_LEX, EXP_ID);
		match('=', NOACTION, EXP_IGUAL);
		constante(PRINT_CONST);
        tp = gettupla(getlast()-1);
        (*tp).type = (*currentp).type;
        if(LITERAL == (*currentp).qual)
        	(*currentp).qual = LITERAL_CONSTANT;
			match(';', NOACTION, EXP_PTOVIRG);
			if(lookahead == IDENTIFICADOR) cont = 1;
			else cont = 0;
		}while(cont);
    }
  case VAR:
    if(lookahead == VAR){
      match(VAR, SECTION_BSS, EXP_VAR);
      do{
			do{
			  qualificador(VAR);
			  match(IDENTIFICADOR, MARK, EXP_ID);
			  if(lookahead == ',') {cont = 1; match(',', NOACTION, EXP_VIRG);}
			  else cont = 0;
			}while(cont);
			match(':', NOACTION, EXP_PTOPTO);
			tipo(PRINT_VAR);
			match(';', NOACTION, EXP_PTOVIRG);
			if(lookahead == IDENTIFICADOR) cont = 1;
			else cont = 0;
      }while(cont);
    }
  }/* switch */
  match(BEGIN, JMP_START, EXP_BEGIN);
  do{
    comando();
    if(lookahead == ';'){cont = 1; match(';', NOACTION, EXP_PTOVIRG);}
    else cont = 0;
  }while(cont);
  match(END, NOACTION, EXP_END);
}

void comando(){
  static int L_If = 0, L_Case = 0, L_While = 0;
  static int L_Repeat = 0, L_For = 0, Next_Label = 0;
  int cont,
    END_IF_LABEL = L_If,
    END_LABEL_CASE = L_Case++,
    END_LABEL_WHILE = L_While,
    END_L_FOR = L_For,
    END_NEXT_LABEL = Next_Label,
    END_L_REPEAT = L_Repeat;
  char *variable;
  action_t act;


  if(match_qualifier(ID_LABEL,FALSE)){
    match(IDENTIFICADOR, PRINT_LABEL, EXP_INT);
	 match(':', NOACTION, EXP_PTOPTO);
  }
  switch(lookahead){
  case BEGIN:
    match(BEGIN, NOACTION, EXP_BEGIN);
    comando();
    while(lookahead == ';') {match(';', NOACTION, EXP_PTOVIRG); comando();}
    match(END, NOACTION,EXP_END);
    break;
  case IF:
    match(IF, NOACTION, EXP_IF);
	 expressao(PUSH);
    match(THEN, NOACTION, EXP_THEN);
    act_then(L_If++);
	 comando();
    if(lookahead == ELSE){
      act_else(L_If, END_IF_LABEL);
      END_IF_LABEL = L_If++;
      match(ELSE, NOACTION, EXP_ELSE);
      comando();
    }
    put_label_if(END_IF_LABEL);
    break;
  case CASE:
    match(CASE, NOACTION, EXP_CASE);
	 expressao(PUSH);
	 match(OF, NOACTION, EXP_OF);
    do{
      constante(EXPRESSION_CASE);
      print_conditional_jmp_label_case(Next_Label);
      while(lookahead == ','){
	     match(',', NOACTION, EXP_VIRG);
        constante(EXPRESSION_CASE);
        print_conditional_jmp_label_case(Next_Label);
      }
      END_NEXT_LABEL = Next_Label++;
      match(':', NOACTION, EXP_PTOPTO);
      comando();
      if(lookahead == ';'){
        jmp_end_label_case(END_LABEL_CASE);
        match(';', NOACTION, EXP_PTOVIRG);
        cont =1;
      }
      else cont = 0;
      print_next_label_case(END_NEXT_LABEL);
    }while(cont);
    match(END, NOACTION,EXP_END);
    put_end_label_case(END_LABEL_CASE);
    break;
  case WHILE:
    match(WHILE, NOACTION, EXP_WHILE);
    put_label_while(L_While);
	 expressao(PUSH);
    match(DO, NOACTION, EXP_DO);
    jmp_when_false(L_While++);
	 comando();
    jmp_to_begin(END_LABEL_WHILE);
    put_end_label_case(END_LABEL_WHILE);
    break;
  case REPEAT:
    match(REPEAT, NOACTION, EXP_REPEAT);
    print_label_repeat(L_Repeat++);
    comando();
    while(lookahead == ';'){
    	match(';', NOACTION, EXP_PTOVIRG);
      comando();
    }
    match(UNTIL, NOACTION, EXP_UNTIL);
    expressao(PUSH);
    jmp_if_false(END_L_REPEAT);
    break;
  case FOR:
    match(FOR, NOACTION, EXP_FOR);
	match_qualifier(ID_VAR,TRUE);
    variable = strdup(lexema);
    match(IDENTIFICADOR, SALVE_VARIABLE, EXP_ID);
	match(ASSIGN, NOACTION, EXP_ASSIGN);
	expressao(PUSH);
    semantic(ATRIBUIR);
    match(TO, NOACTION, EXP_TO);
    put_label_for(L_For++);
	 expressao(PUSH);
    /* se for <= continua, senão cai fora */
    verifica_contador(variable, END_L_FOR);
	match(DO, NOACTION, EXP_DO);
	comando();
    /* incrementa o valor do lexema e põe o jmp para o início */
    jmp_label_for(variable, END_L_FOR);
    break;
  case GOTO:
    match(GOTO, NOACTION, EXP_GOTO);
	match(IDENTIFICADOR, INCONDITIONAL_JMP, EXP_ID);
    break;
  default:
/*    if(match_qualifier(ID_FUN,0)){
      match(IDENTIFICADOR, NOACTION, EXP_ID);
	   match(ASSIGN, NOACTION, EXP_ASSIGN);
      expressao(NOACTION);
    }
    else */if(match_qualifier(ID_PRO,0)){
	    act = (!strcmp("WRITE",lexema) ? PUT_BUFFER_OUT :
			     !strcmp("READ",lexema)  ? PUT_BUFFER_IN  :
			      NOACTION);
       match(IDENTIFICADOR, SALVE_PROC, EXP_ID);
       if(lookahead == '('){
			match('(', NOACTION, EXP_ABRPAR);
			expressao(act);
			while(lookahead == ','){
         	match(',', NOACTION, EXP_VIRG);
            expressao(act);
         }
		   match(')', NOACTION, EXP_FECPAR);
         semantic(CALL_PRO);
       }
    }
    else if(variavel(SALVE_VARIABLE)){
      match(ASSIGN, NOACTION, EXP_ASSIGN);
      expressao(PUSH);
      semantic(ATRIBUIR);
    }
    else
      ;
  }/* switch */
}

void expressao(action_t action){
  action_t act = NOACTION;

  expressao_simples(action);
  switch(lookahead){
  case '=':
	act = IGUAL;
  case '#':
	if(!act) act = DIFERENTE;
  case '<':
	if(!act) act = MENOR_QUE;
  case LE:
	if(!act) act = MENOR_IGUAL_QUE;
  case GE:
	if(!act) act = MAIOR_IGUAL_QUE;
  case '>':
	if(!act) act = MAIOR_QUE;
	match(lookahead, NOACTION, NONE);
	expressao_simples(action);
	semantic(act);
   /* jogo o resultado da expressão no acumulador */
    semantic(POP2Acc);
  default:
    ;
  }
}

void expressao_simples(action_t action){
  action_t act = NOACTION;
  char t = '-';

  switch(lookahead){
  case '+':
  	 t = '+';
  case '-':
  	 match(lookahead, NOACTION, NONE);
  default:
    termo(action);
    if(t == '-') semantic(OPOSTO);
    while(1){
      switch(lookahead){
      case '+':
			act = PLUS;
	   case '-':
			if(!act) act = MINUS;
	   case OR:
	   	if(!act) act = LOGIC_OR;
			match(lookahead, NOACTION, NONE);
         termo(action);
			semantic(act);
			break;
      default:
			return;
      }/* switch interno */
    }  /*     while      */
  }    /* switch externo */
}

void termo(action_t action){
  action_t act = NOACTION;

  fator(action);
  while(1){
    switch(lookahead){
    case '*':
	 	act = MULTIPLICA;
    case '/':
	 	if(!act) act = DIVIDE_FLOAT;
	 case DIV:
	 	if(!act) act = DIVIDE_INT;
	 case MOD:
    	if(!act) act = DIVIDE_RESTO;
	 case '&':
	 	if(!act) act = LOGIC_AND;
      match(lookahead, NOACTION, NONE);
      fator(action);
	 	semantic(act);
	 	break;
    default:
      return;
    }
  }
}

void fator(action_t action){
  switch(lookahead){
  case '(':
     match('(', NOACTION, EXP_ABRPAR);
	 expressao(action);
	 push("%eax");
	 match(')', NOACTION, EXP_FECPAR);
	 break;
  case NOT:
     match(NOT, NOACTION, NONE);
	 fator(action);
	 semantic(NEGAR);
	 break;
     /* CONST_SEM_SINAL */
  case NUMBER:
     match(NUMBER, action, EXP_NUMBER);
     break;
  case LITERAL_CHAR:
  case LITERAL_TEXT:
     match(lookahead, action, EXP_LITERAL);
     break;
  default:
    if(match_qualifier(ID_CON,FALSE))
      match(IDENTIFICADOR, action, EXP_ID);
    /* VARIÁVEL */
    else if(!variavel(action))
      /* se não for nenhuma das possibilidades acima
       * um erro é gerado */
      error(SINTAX_ERR);
  }/* switch */
}

boolean_t variavel(action_t action){
  boolean_t ok = FALSE;
  if(match_qualifier(ID_VAR,FALSE)){
    match(IDENTIFICADOR, action, EXP_ID);
    ok = TRUE;
  }
  return ok;
}

void tipo(action_t act){
  while(1){
    switch(lookahead){
    case ARQUIVO:
      match(ARQUIVO, NOACTION, NONE);
	   match(OF, NOACTION, EXP_OF);
      break;
    default:
      tipo_simples(act);
      return;
    }
  }/* while */
}

void tipo_simples(action_t act){
  int t = '-';

  switch(lookahead){
    /* constante */
  case LITERAL_CHAR:
	 match(LITERAL_CHAR, act, NONE);
	 match(PTO_PTO, NOACTION, EXP_PTOPTO);
	 constante(act);
	 break;
  case LITERAL_TEXT:
     match(LITERAL_TEXT, act, NONE);
	 match(PTO_PTO, NOACTION, EXP_PTOPTO);
	 constante(act);
	 break;
  case '+':
    t = '+';
  case '-':
    match(lookahead, NOACTION, NONE);
  case NUMBER:
    if(lookahead == NUMBER) {
	   match(NUMBER, act, EXP_NUMBER);
	   match(PTO_PTO, NOACTION, EXP_PTOPTO);
   	constante(NOACTION);
      if(t == '-') semantic(OPOSTO);
	   break;
    }
  default:
    if(match_qualifier(ID_CON,FALSE)){
       match(IDENTIFICADOR, act, EXP_ID);
	    match(PTO_PTO, NOACTION, EXP_PTOPTO);
       constante(NOACTION);
	   break;
    }
    if(match_qualifier(ID_TIP,FALSE)){
       match(IDENTIFICADOR, act, EXP_ID);
	   break;
    }
    else
      /* Se nenhuma das possibilidades for satisfeita ocorrerá um erro */
      error(SINTAX_ERR);
  }
}

void constante(action_t act){
  int t = '+';
  switch(lookahead){
  case LITERAL_CHAR:
  case LITERAL_TEXT:
    match(lookahead, act, NONE);
    break;
  case '-':
    t = '-';
  case '+':
    match(lookahead, NOACTION, NONE);
  case NUMBER:
    match(NUMBER, act, EXP_NUMBER);
    if(t == '-') semantic(OPOSTO);
    break;
  default:
    if(match_qualifier(ID_CON,FALSE)){
      match(IDENTIFICADOR, act, EXP_ID);break;
    }
    else
      /* Se nenhuma das possibilidades for satisfeita ocorrerá um erro */
      error(SINTAX_ERR);
  }
}
