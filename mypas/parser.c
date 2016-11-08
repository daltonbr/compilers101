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
 * 	| IF expr THEN body { ELIF expr THEN body } [ ELSE body ] ENDIF
 * 	| WHILE expr DO body ENDDO 
 *	| DO body WHILE expr
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
			repstmt();
			break;
		case ID: case NUM: case'(':
			expr();
			break;
		default:
			; /*<empty>*/
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
		} while(lookeahead == ID);
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
	stmt();
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
	/*[[*/symvec[i] = malloc(sizeof(lexeme) + 1;
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

/*
 *ifstmt -> IF expr THEN body { ELIF expr THEN body } [ ELSE body ] ENDIF      // { é para fecho de klein  e [ é para opcionalidade (0 ou 1)
 */
void ifstmt(void)
{
	match(IF);
	printf("after match \n: %c", lookahead);
	expr();
	match(THEN);
	body();
	while (lookahead == ELIF) {
		match(ELIF);
		expr();
		match(THEN);
		body();
	}
	if(lookahead == ELSE) {
		match(ELSE);
		body();
	}
	match(ENDIF);
}

/*
 * whilestmt -> ...
 */
void whilestmt(void)
{
	match(WHILE); expr();
	match(DO); body();
	match(DONE);
}

/*
 * dostmt
 */
void repstmt
{
	(void)printf("ID: %c", lookahead);

	match(DO); body(); match(WHILE); expr();
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
<<<<<<< HEAD
 * expr -> ['-'] term { addop term } */
 /*
  * OP  | BOOLEAN | NUMERIC | 
  * NOT |    X
  * OR  |
  * AND |
  */
int expr (int inherited_type)
{
    /*[[*/ int varlocality, lvalue = 0, acctype = inherited_type, syntype, ltype, rtype; /*]]*/ 
=======
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
 * REAL   |     -     |   REAL  |  REAL  |  REAL  |
 * DOUBLE |     -     |  DOUBLE | DOUBLE | DOUBLE |
 *
 */
void expr (nt inherited_type)
{
    /*[[*/ int varlocality, lvalue = 0, acctype = inherited_type, syntype; /*]]*/ 
>>>>>>> d3cbfe8b7de001d4c23331cd35a8305b48519db6
	int p_count = 0;
	if(lookahead == '-') {
		match('-');
		/*[[*/
		if(acctype == BOOLEAN) {
			fprintf(stderr,"incompatible unary operator: FATAL ERROR.\n");
			// TODO: need to set a flag to this fatal error
		} else if(acctype == 0){
			acctype = INTEGER;
		}
		/*]]*/
	} else if (lookahead == NOT) {
		match(NOT);
		/*[[*/
		if(acctype > BOOLEAN) {
			fprintf(stderr,"incompatible unary operator: FATAL ERROR.\n");
		}
		acctype = BOOLEAN;
		/*]]*/
	}
	E_entry:	
	T_entry:
	F_entry:
		switch (lookahead) {
			case ID:
				/*[[*/ varlocality = symtab_lookup(lexeme);
                if (varlocality < 0) {
                    fprintf(stderr, "parser: %s not declared - fatal error!\n", lexeme);
                    //TODO: need to set a flag to this fatal error
                } else {
                    syntype = symtab[varlocality][1];
                }
                /*]]*/
				match (ID);
				if (lookahead == ASGN) { //ASGN = ":="
					/* located variable is LVALUE */
                    /*[[*/ lvalue = 1; 
                    ltype = syntype;
                    /*[[*/ 
                    match(ASGN);
					/*[[*/
                    rtype = 
                    /*]]*/
                    expr(/*[[*/ltype/*]]*/);
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
				match(FLTCONST);
				break;
			case INTCONST:
				match(INTCONST);
				break;
			default:
				match('(');
				/*[[*/syntype = /*]]*/expr();
				/*[[*/if(iscompatible(syntype, acctype){
					acctype = max(acctype, syntype);
				} else {
					fprintf(stderr, "parenthesizes type incompatible with accumulated");
				}/*]]*/
				match(')');

		}
	if(mulop()) goto F_entry;
	if(addop()) goto T_entry;
	
	if(p_count > 0)
		printf("MISSING )\n");

    /* expression ends down here */
    /*[[*/ if (varlocality > -1 && lvalue) { 
    fprintf(object, "\tmovl %%eax,%s\n", symtab_stream + symtab[varlocality][0]);
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