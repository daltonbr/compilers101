/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <tokens.h>
#include <parser.h>
#include <keywords.h>
#include <symtab.h>
#include <macros.h>

/*************************** LL(1) grammar definition ******************************
 *
 * mypas -> body EOF
 */
void mypas(void)
{
	body();
	match(EOF);
}

/*
 * body -> stmt { stmtsep stmt }
 */
/*
 * body -> declarative imperative
 */
void body(void)
{
	declarative();
	imperative();
}



/*
 * stmtsep -> ; | EOL
 */
int stmtsep(void)
{
	switch(lookahead){
		case ';':case '\n':   /// fazer em casa o \n
			match(lookahead); return 1;
	}
	return 0;
}

/*
 * stmt -> imperative
 * 	| IF expr THEN stmtlist { ELIF expr THEN stmtlist } [ ELSE stmtlist ] ENDIF
 * 	| WHILE superexpr DO stmtlist 
 *	| DO stmtlist WHILE superexpr
 *	| expr
 *	| <empty>
 */
void stmt(void)
{
	printf("CMD: %d", lookahead);
	switch(lookahead){
		case BEGIN:
			imperative();
		case IF:
			ifstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		case REPEAT:
			repeatstmt();
			break;
		case INT:
		case ID:	// hereafter we expect FIRST(expr)
		case FLTCONST:
		case INTCONST:
		case TRUE:	// TODO: colocar isso no analisador lexico
		case FALSE:	// TODO: isso também
		case NOT:
		case '-':
		case '(':
			expr(0);
			break;
		default:
			/*<epsilon>*/;    // Emulating empty word
	}
}

/*
 * WRITE -> WRITE ( " string " )
 */

/*
 * declarative -> [
 * 		   VAR namelist ':' vartype ';' ||
 * 		     { namelist ':' vartype ';' }
 * 		  ]
 *		  { sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' }
 */

void declarative(void)
{
	/*
	 * vardef -> VAR namelist ':" vartype ';' || vardef.symtab <- forall symbol in namelist.name do
	 * 								symtab_append(symbol,vartype.type)
	 * 							      end do
	 */
	
	if(lookahead == VAR) {
		match(VAR);
		do {
			/*[[*/int type, i/*]]*/;
			/*[[*/char **namev = /*]]*/ namelist();
			match(':');
			/*[[*/type = /*]]*/ vartype();
			/*[[*/for(i=0; namev[i];i++)symtab_append(namev[i],type)/*]]*/;
			match(';');
		} while(lookahead == ID);
	}
	while(lookahead == PROCEDURE || lookahead == FUNCTION) {
		match(lookahead);
		match(ID);
		parmdef();
		if(lookahead == ':') {
			match(':');
			fnctype();
			match(';');
			body();
			match(';');
		}
	}
}

/*
 * imperative -> BEGIN stmtlist END
 */
void
imperative(void)
{
	match(BEGIN);
	stmtlist();
	match(END);
}

void parmdef(void)
{
	if(lookahead == '(') {
		match('(');
_par_begin:
		if(lookahead == VAR) {
			match(VAR);
		}
		namelist();
		match(':');
		vartype();
		if(lookahead == ';') {
			match(';');
			goto _par_begin;				
		}
	}	
}

/*
 * namelist -> ID { , ID}
 */
#define MAX_ARG_NUM 1024
char **
namelist(void)
{
	/*[[*/char **symvec = calloc(MAX_ARG_NUM, sizeof(char **));
	int i = 0/*]]*/;
	_namelist_begin:
	/*[[*/symvec[i] = malloc(strlen(lexeme) + 1); //TODO: strlen funciona? é pra pegar o tamanho do lexeme?
	strcpy(symvec[i], lexeme);
	i++/*]]*/;
	match(ID);
	if(lookahead == ',') {
		match(',');
		goto _namelist_begin;
	}
	/*[[*/return symvec;/*]]*/
}

void stmtlist(void)
{
	_stmtlist_begin:
	stmt();
	if(lookahead == ';') {
		match(';');
		goto _stmtlist_begin;
	}
}

void vartype(void)
{
	switch(lookahead){
		case INTEGER:
			match(INTEGER);
			break;
		case REAL:
			match(REAL);
			break;
		default:
			match( BOOLEAN);
    }
}

void fnctype(void)
{
	switch(lookahead){
		case INTEGER:
			match(INTEGER);
			break;
		case REAL:
			match(REAL);
			break;
		default:
			match( BOOLEAN);
    }
}

int labelcounter = 1;		// global var to label in machine code

/*
 *ifstmt -> IF expr THEN body { ELIF expr THEN body } [ ELSE body ] ENDIF      // { é para fecho de klein  e [ é para opcionalidade (0 ou 1)
 */
void ifstmt(void)
{
	int syntype;
	int _endif, _else;
	match(IF);
	printf("after match \n: %c", lookahead);
	syntype = superexpr(BOOLEAN); // TODO: check if is boolean
//	if(superexpr(BOOLEAN) < 0) // TODO: deu pau, escreve o erro tipo not boolean
//	fprintf(object,"\tjz .L%d\n", _endif = _else = labelcounter++); TODO: criar arquivo object e descomentar os outros fprintf
	_endif = _else = gofalse(labelcounter++);
	match(THEN);
	body(); // eraldo colocou stmt(); mas não tem o ELIF
	while (lookahead == ELIF) {
		match(ELIF);
		syntype = superexpr(BOOLEAN);		
		match(THEN);
		body();
	}
	if(lookahead == ELSE) {
		match(ELSE);
//		fprintf(object,"\tjmp .L%d\n", _endif = labelcounter++);
//		fprintf(object,".L%d:\n", _else);
		_endif = jump(labelcounter++);
		mklabel(_else);
		/**/
		body();
	}
	match(ENDIF);
//	fprintf(object,".L%d:\n", _endif);
	mklabel(_endif);
}

/*
 * whilestmt -> ...
 *	WHILE superexpr DO stmtlist 
 */
void whilestmt(void)
{
	int syntype;
	/**/int while_head, while_tail/**/;
	
	match(WHILE); 
	/**/mklabel(while_head = labelcounter++);   /// cria a cabeça do while -> faz o ponto de entrada do while
	syntype = superexpr(BOOLEAN);
//	if(superexpr(BOOLEAN) < 0) // TODO: deu pau, escreve o erro tipo not boolean
	/**/gofalse(while_tail)/**/;
	match(DO); stmt();
	/**/jump(while_head = labelcounter++)/**/;
	/**/mklabel(while_tail)/**/;// aqui vai a saída do while
	match(DONE);
	/*
	 *    | while |
	 *        |
	 *        v--<---
	 *       / \     |
	 *  --- /exp\    |
	 * |    \   /    |
	 * |     \ /     |
	 * |      |      |
	 * |      v      |
	 * |   | stmt |  |
	 * |      |      |
	 * |      -------
	 * |
	 *  ------
	 *       |
	 *       v
	 * | end-while |
     */
}

/*
 * dostmt
 */
void repeatstmt(void)
{
	int syntype;	
	(void)printf("ID: %c", lookahead);

	match(DO); stmt(); match(WHILE); 
	syntype = superexpr(BOOLEAN);
//	if(superexpr(BOOLEAN) < 0) // TODO: deu pau, escreve o erro tipo not boolean
}

/*
 *
 * expr -> term {addop term}
 *
 * term -> fact {mulop fact}
 *
 * fact -> vrbl | cons | ( expr )
 *
 * vrbl -> ID
 *
 * cons -> DEC
 *
 * addop -> '+' | '-'
 *
 * mulop -> '*' | '/'
 *
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 * DEC = [1-9][0-9]* | 0
 *
 **********************************************************************************/

/***************************** LL(1) grammar emulation *****************************
 *
 * expr -> ['-'] term { addop term }
 */
/*
 * OP     |  BOOLEAN  | NUMERIC | 
 * =============================
 * NOT    |     X     |    -    |
 * OR     |     X     |    -    |
 * AND    |     X     |    -    |
 * CHS    |     -     |    X    |    // negate CHS CHangeSignal
 * '+''-' |     -     |    X    |
 * '*''/' |     -     |    X    |
 * DIV    |     -     | INTEGER |
 * MOD    |     -     | INTEGER |
 * RELOP  |BOOL x BOOL|NUM x NUM|
 *
 *
 * EXPRESS|  INTEGER  |   REAL  | DOUBLE |
 * ===================================
 * INTEGER|  INTEGER  |   REAL  | DOUBLE |
 * REAL   |    REAL   |   REAL  | DOUBLE |
 * DOUBLE |  DOUBLE   |  DOUBLE | DOUBLE |
 *
 *
 * LVALUE |  BOOLEAN  | INTEGER |  REAL  | DOUBLE |
 * =============================================
 * BOOLEAN|  BOOLEAN  |    -    |   -    |   -    |
 * INTEGER|     -     | INTEGER |  CAST  |   -    |
 * REAL   |     -     |   REAL  |  REAL  |   -    |
 * DOUBLE |     -     |  DOUBLE | DOUBLE | DOUBLE |
 *
 */

int iscompatible(int ltype, int rtype)
{
	switch(ltype) {
		case BOOLEAN:
			if(rtype == ltype) {
				return ltype;
			}
			break;
		case INTEGER:
			if(rtype = ltype) {
				return ltype;
			}
			break;
		case REAL:
			switch(rtype) {
				case INTEGER:
				case REAL:
					return ltype;
			}
			break;
		case DOUBLE:
			switch(rtype) {
				case INTEGER:
				case REAL:
				case DOUBLE:
					return ltype;
			}
	}
	return 0;
}

void isrelop()
{
	switch(lookahead) {
		case '>':
			match('>');  // match é para sintaxe! Análise léxica não se usa match
			if(lookahead == '='){
				match('=');
				return GEQ;
			}
			return '>';
		case '<':
			match('<');
			if(lookahead == '='){
				match('=');
				return LEQ;
			}else if(lookahead == '>'){
				match('>');
				return NEQ;
			}
			return '<';
		case '=':
			match('=');
			return '=';
	}
	return 0;
}

/* syntax: superexpr -> expr [ relop expr ]*/
int superexpr(int inherited_type)
{
	int t1, t2;
	t1 = expr(inherited_type);
	if(isrelop()) {
		t2 = expr(t1);
		// TODO: check compatibility 
	}
	return min(BOOLEAN,t2);
}
void expr (int inherited_type)
{
    /*[[*/ int varlocality, lvalue_seen = 0, acctype = inherited_type, syntype, ltype, rtype; /*]]*/ 
	int p_count = 0;
	if(lookahead == '-') {
		match('-');
		/*[[*/
		if(acctype == BOOLEAN) {
			fprintf(stderr,"incompatible unary operator: FATAL ERROR.\n");
			// TODO: need to set a flag to this fatal error
		} else if(acctype == 0) acctype = INTEGER;
		/*]]*/
	} else if (lookahead == NOT) {
		match(NOT);
		/*[[*/
		if(acctype > BOOLEAN) fprintf(stderr,"incompatible unary operator: FATAL ERROR.\n");
		acctype = BOOLEAN;
		/*]]*/
	}
	E_entry:	
	T_entry:
	F_entry:
		switch (lookahead) {
			case ID:
				/*[[*/
                varlocality = symtab_lookup(lexeme);
                if (varlocality < 0) {
                    fprintf(stderr, "parser: %s not declared - fatal error!\n", lexeme);
					syntype = -1;
					//TODO: need to set a flag to this fatal error
                } else {
					syntype = symtab[varlocality][1];
                }
                /*]]*/
				match (ID);
				if (lookahead == ASGN) { 		//ASGN = ":="
					/* located variable is LTYPE */
					/*[[*/ 
					lvalue_seen = 1; // TODO: declare this locally
					ltype = syntype;
                    /*]]*/ 
		            match(ASGN);
					/*[[*/rtype = /*]]*/superexpr(/*[[*/ltype/*]]*/);
					/*[[*/
					if(iscompatible(ltype, rtype)){
						acctype = max(rtype,acctype);
					}else{
						acctype = -1; // sintax error
					}
					/*]]*/
				}
				/*[[*/
                else if(varlocality > -1) {  //TODO: this is really necessary
                    fprintf(object,"\tpushl %%eax\n\tmov %s,%%eax\n",
	                symtab_stream + symtab[varlocality][0]);
                }
		        /*]]*/
				break;
/* BEGIN ERALDO NÂO TEM ESSA PARTE */
			case NUM:
				printf("decimal: %c", lookahead);
				match (NUM);
				
				break;
			case '(':
				printf("(: %c", lookahead);
				match ('('); p_count++;
				goto E_entry;
				break;
			case ')':
				printf("): %c", lookahead);
				match (')'); p_count--;
				if(p_count < 0) {printf("MISSING (\n"); exit(0);}
				goto T_entry;
				break;			
/* FIM ERALDO NÂO TEM ESSA PARTE */
			case FLTCONST:
				{
					float lexval = atof(lexeme);
					char *fltIEEE = maloc(sizeof(lexeme)+1);
					sprintf(fltIEEE, "$%i", *((int *)&lexval));
					rmovel(fltIEEE);
				}
				match(FLTCONST);
				break;
			case INTCONST:
				match(INTCONST);
				break;
			default:
				match('(');
				/*[[*/syntype = /*]]*/superexpr(0);
				/*[[*/
                if(iscompatible(syntype, acctype)){
					acctype = max(acctype, syntype);
				} else {
					fprintf(stderr, "parenthesizes type incompatible with accumulated");
				}
				/*]]*/
				match(')');
		}
	if(mulop()) goto F_entry;
	if(addop()) goto T_entry;
	
	if(p_count > 0)
		printf("MISSING )\n");

    /* expression ends down here */
    /*[[*/ if (lvalue_seen && rlocality > -1) {
		switch(ltype){
			case INTEGER:case REAL:
				lmovel(symtab_stream + symtab[varlocality[0]]);
				break;
			case DOUBLE:
				lmoveq(symtab_stream + symtab[varlocality[0]]);
				break;
			default:
				; // desenvolver algo aqui
		}
    }

    /*]]*/  
}
/*
 * vrbl -> ID
 *
 * cons -> DEC
*
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 * DEC = [1-9][0-9]* | 0
 *
 * addop -> '+' | '-' */
int addop (void)
{
	switch(lookahead){
	case '+':
			match('+'); return '+';
	case '-':
			match('-'); return '-';
    case OR:
        match(OR); return OR;
	}
	return 0;
}

/*
 * mulop -> '*' | '/' */
int mulop (void)
{
	switch(lookahead){
		case '*':
			match('*'); return '*';
		case '/':
			match('/'); return '/';
		case AND:
			match(AND); return AND;
	}
	return 0;
}
/***************************** lexer-to-parser interface **************************/

int lookahead; // @ local

void match (int expected)
{
	 if ( expected == lookahead) {
		 lookahead = gettoken (source);
	 } else {
		 fprintf(stderr,"parser: token mismatch error. found # %d ",
		 	lookahead);
		 fprintf(stderr,"whereas expected # %d\n",
		 	expected);
		 exit (SYNTAX_ERR);
	 }
}
