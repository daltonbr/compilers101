/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <keywords.h>


/*************************** LL(1) grammar definition ******************************
 *
 * progcalc -> cmdblock EOF
 */
void progcalc(void)
{
	cmdblock();
	match(EOF);
}

/*
 * cmdblock -> cmd { cmdsep cmd }
 */
void cmdblock(void)
{
	do {
		cmd();
	} while(cmdsep());
}

/*
 * cmdsep -> ; | EOL
 */
int cmdsep(void)
{
	switch(lookahead){
		case ';':case '\n':
			match(lookahead); return 1;
	}
	return 0;
}

/*
 * cmd -> IF expr THEN cmdblock [ ELSE cmdblock ] ENDIF
 * 	| WHILE expr DO cmdblock ENDDO 
 *	| DO cmdblock WHILE expr
 *	| expr
 *	| <empty>
 */
void cmd(void)
{
	switch(lookahead){
		case IF:
			ifstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		case DO:
			dostmt();
			break;
		case ID: case DEC: case'(':
			expr();
			break;
	}
}

/*
 *ifstmt -> ...
 */
void ifstmt(void)
{
	match(IF); expr(); match(THEN); cmdblock();
	if(lookahead == ELSE) {
		match(ELSE); cmdblock();
	}
	match(ENDIF);
}

/*
 * whilestmt -> ...
 */
void whilestmt(void)
{
	match(WHILE); expr();
	match(DO); cmdblock();
	match(ENDWHILE);
}

/*
 * dostmt
 */
void dostmt(void)
{
	match(DO); cmdblock(); match(WHILE); expr();
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
 * expr -> term { addop term } */
void expr (void)
{
	term(); while(addop()) { term(); }
}
/*
 * term -> fact { mulop fact } */
void term (void)
{
	fact(); while(mulop()) { fact(); }
}
/*
 * fact -> vrbl | cons | ( expr ) */
void fact (void)
{
	switch (lookahead) {
	case ID:
		match (ID); break;
	case DEC:
		match (DEC); break;
	default:
		match ('('); expr(); match (')');
	}
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

