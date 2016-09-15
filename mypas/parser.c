/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <keywords.h>


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
 * stmt -> IF expr THEN body { ELIF expr THEN body } [ ELSE body ] ENDIF
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
		case ID: case DEC: case'(':
			expr();
			break;
		
	}
}
void declarative(void)
{
	if(lookahead == VAR) {
		match(VAR);
		do {
			namelist();
			match(';');
			vartype();
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

void imperative(void)
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

void namelist(void)
{
	_namelist_begin:
	match(ID);
	if(lookahead == ',') {
		match(',');
		goto _namelist_begin;
	}
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
	match(DEC);
}

void fnctype(void)
{
	match(DEC);
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
 * expr -> ['-'] term { addop term } */
void expr (void)
{
	int p_count = 0;
	if(lookahead == '-') {
		match('-');
	}
	E_entry:	
	T_entry:
	F_entry:
		switch (lookahead) {
			case ID:
				printf("ID: %c", lookahead);
				match (ID);
				if (lookahead == '=') {
					match('=');
					expr();
				}
				break;
			case DEC:
				printf("decimal: %c", lookahead);
				match (DEC);
				
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
		}
	if(mulop()) goto F_entry;
	if(addop()) goto T_entry;
	
	if(p_count > 0)
		printf("MISSING )\n");
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

