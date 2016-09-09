/**@<parser.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <interpreter.h>


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
 *      source language        ||          object language
 * -----------------------------------------------------------------------
 * expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
 * -----------------------------------------------------------------------
 * expr -> term { addop term [[ printf(addop.pf); ]] }
 */
int is_cmdsep(char c)
{
	if(c == ';' || c == '\n')
		return 1;
	return 0;	 
}

void mybc(void)
{
	expr();

	while(is_cmdsep(lookahead)) {
                clear_accumulator();
		clear_stack();
		match (lookahead);
		expr();
	}
	match(EOF);
}

void expr (void)
{       /**/int op, neg = 0/**/;
	if (lookahead == '-') { match ('-'); /**/neg = '-'/**/;}
        term(); /**/if(neg){printf("<+/-> ");}/**/
	while(( op = addop()) ) { /**/push_stack()/**/; term();/**/operation(op)/**/;}
}
/*
 * term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }
 */
void term (void)
{       /**/int op/**/;
        fact();
	while(( op = mulop()) ) { /**/push_stack()/**/; fact();/**/operation(op)/**/;}
}
/*
 * previous version: fact -> vrbl | cons | ( expr ) || fact.pf := expr.pf
 * new one: fact -> ID [ = expr ] | DEC | ( expr )
 */
void fact (void)
{/**/char bkplexeme[MAXSIZE_LEXEME+1]/**/;
        switch (lookahead) {
        case ID:
		strcpy(bkplexeme, lexeme);
		match (ID);
		if (lookahead == '='){
			match('='); expr();
			set_value_to_id(bkplexeme);
			/**/printf("%s <store> ",bkplexeme)/**/;
		} else {
			next_value_with_id(bkplexeme);
			/**/printf("%s ",bkplexeme)/**/;
		}
		break;
        case DEC:
		next_value((double)atoi(lexeme));
                /**/printf("%s ",lexeme)/**/; match (DEC); break;
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
         if ( expected == lookahead ) {
                 lookahead = gettoken (source);
         } else {
                 fprintf(stderr,"parser: token mismatch error. found # %d ",
                        lookahead);
                 fprintf(stderr,"whereas expected # %d\n",
                        expected);
                 exit (SYNTAX_ERR);
         }
 }

