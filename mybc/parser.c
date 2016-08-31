/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>



/*************************** LL(1) grammar definition ******************************
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
	int p_count = 0;
	E_entry:
	T_entry:
	F_entry:
		switch (lookahead) {
			case ID:
				match (ID);
				break;
			case DEC:
				match (DEC);
				break;
			case '(':
				match ('('); p_count++;
				goto E_entry;
				break;
			case ')':
				match (')'); p_count--;
				if(p_count <0) {printf("MISSING ( \n"); exit(0);}
				break;			
		}
	if(mulop()) goto F_entry;
	if(addop()) goto T_entry;
	if(lookahead == EOF)
	{
	 if (p_count > 0) printf("MISSING ) %d \n", p_count);
	 exit(0);
	}
	goto F_entry;  // is this conceptually correct?
	
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

