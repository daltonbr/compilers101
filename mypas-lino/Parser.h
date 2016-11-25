/*
 * Parser.h
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

/*
 * standard input buffer is here:
 */
extern FILE *tape;
extern int lookahead;

int is_simple_type();
int is_constant();
int is_unsigned_number();
int is_expression();
int is_simple_expression();
int is_term();
int is_factor();
int is_unsigned_constant();

void program();
void block();
int simple_type();
void statement();
int expression(int);
int simple_expression(int);
int term(int);
int factor();
int unsigned_constant();


/* Semantic begin */
#define NUMERIC_TYPE INTEGER+REAL+DOUBLE
int type_check(int, int);
int op_type(int, int);
/* Semantic end */

/*


void heading();
void parameter_list();
void type_id();
void field_list();

void variable();

void constant();
void unsigned_number();
*/
/*
 * gettoken is essentially the lexer method, which abstracts * a finite
 * automata archive -- implemented in lexer.c
 */
int get_token(FILE * tape);

#endif /* PARSER_H_ */
