#include <parser.h>

/*************************** LL(1) grammar definition ******************************
 *
 * expr -> term rest
 *
 * rest -> addop term rest | <>
 *
 * term -> fact quoc
 *
 * quoc -> mulop fact quoc | <>
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
 * expr -> term rest */
void expr (void)
{
	term(); rest();
}

/*
 * rest -> addop term rest | <> */
void rest (void)
{
	if (addop()) {
		term(); rest();
	}
}
/*
 * term -> fact quoc */
void term (void)
{
	fact(); quoc();
}
/*
 * quoc -> mulop fact quoc | <> */
void quoc (void)
{
	if (mulop()) {
		fact(); quoc();
	}
}
/*
 * fact -> vrbl | cons | ( expr ) */
void fact (void)
{
	switch (lookahead) {
	ID:
		match (ID); break;
	DEC:
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
		'+':
			match('+'); return '+';
		'-': 
			match('-'); return '-';
	}
	return 0;
}

/*
 * mulop -> '*' | '/' */
int mulop (void)
{
	switch(lookahead){
		'*':
			match('*'); return '*';
		'/': 
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
		 fprintf(stderr,"parser: token mismatch error # %d... exiting\n",
		 	lookahead);
		 exit (SYNTAX_ERR);
	 }
 }

