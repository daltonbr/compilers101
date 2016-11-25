/*
 * Parser.c
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tokens.h"
#include "Lexer.h"
#include "Parser.h"
#include "Keywords.h"
#include "ErrorHandler.h"
#include "Symtab.h"
/** lexer-to-parser interface **/

int lookahead;
extern int line_count;
int lbl_counter = 1;

/* Semantic begin */

int type_check(int refer_type, int current_type) {
	if (refer_type < 0) return refer_type;

	if (refer_type == 0) return current_type;

	switch(current_type) {
	case DOUBLE:
		switch(refer_type) {
		case DOUBLE:
		case REAL:
		case INTEGER:
			return DOUBLE;
		}
	case REAL:
		switch(refer_type) {
		case DOUBLE:
			return DOUBLE;
		case REAL:
		case INTEGER:
			return REAL;
		}
	case INTEGER:
		switch(refer_type) {
		case DOUBLE:
		case REAL:
		case INTEGER:
			return refer_type;
		}

		if(refer_type == NUMERIC_TYPE) return current_type;
		break;
	case BOOLEAN:
		if (refer_type == current_type) return current_type;
		break;
	}
	return INVALID_TYPE;
}
int op_type(int operator, int type) {
	if (operator == DIV || operator == MOD) {
		if (type != INTEGER) {
			return INVALID_OPERAND;
		}
	}
	return type;
}


/* Semantic end */

void match(int predicted) {
	if (predicted == lookahead) {
		if (lookahead != EOF) {
			lookahead = get_token(tape);
		}
	} else {
		fprintf(stderr, "ERROR: Unexpected token '%s' on line %d. exiting\n", lexeme, line_count);
		exit(-1);
	}
}

void program() {
	match(PROGRAM);
	match(ID);
	match('(');
	match(ID);
	while(lookahead == ',') {
		match(',');
		match(ID);
	}
	match(')');
	match(';');
	block();
	match('.');
}

void type_id() {
	switch(lookahead) {
	case INTEGER:
	case REAL:
	case DOUBLE:
	case BOOLEAN:
		match(lookahead);
		break;
	default:
		match(ID);
	}
}

void block() {
	/**/int var_type,
			first_var,
			last_var/**/;
	switch(lookahead) {
	case VAR:
		match(VAR);
_block_var_id:
		/**/first_var = last_var = symtab_append(lexeme);/**/
		match(ID);
		while (lookahead == ',') {
			match(',');
			/**/last_var = symtab_append(lexeme);/**/
			match(ID);
		}

		match(':');
		var_type = simple_type();

		/**/symtab_settype(first_var, last_var, var_type);/**/
		match(';');

		if (lookahead == ID) {
			goto _block_var_id;
		}

	default:
		match(BEGIN);
		statement();
		while(lookahead == ';') {
			match(';');
			statement();
		}
		match(END);

		break;
	}
}

void field_list() {

	switch(lookahead) {
	case ID:
		match(ID);
		while (lookahead == ',') {
			match(',');
			match(ID);
		}
		match(':');
		simple_type();

		if (lookahead == ';') {
			match(';');
			field_list();
		}
		break;
	default:
		//OK...
		break;
	}
}

int is_simple_type() {
	switch(lookahead) {
		case '(':
		case INTEGER:
		case REAL:
		case DOUBLE:
		case BOOLEAN:
		case ID:
			return 1;
		default:
			return is_constant();
	}
}

int simple_type() {
	switch(lookahead) {
	case INTEGER:
		match(lookahead);
		return INTEGER;
	case REAL:
		match(lookahead);
		return REAL;
	case DOUBLE:
		match(lookahead);
		return DOUBLE;
	default:
		match(BOOLEAN);
		return BOOLEAN;
	}
}

void statement() {
	/**/int var_type,
			sym_pos,
			lbl_endif,
			lbl_else/**/;
	/**/char var_name[MAX_ID_LENGTH]/**/;

	if (lookahead == UINT) {
		match(UINT);
		match(':');
	}

	switch(lookahead) {
	case ID:
		if ((sym_pos = symtab_lookup(lexeme)) < 0) {
			fatal_error(SYMBOL_NOT_FOUND, lexeme);
		} else {
			var_type = symtab_descriptor[sym_pos][1];
		}
		strcpy(var_name, lexeme);
		match(ID);
		if(lookahead == ASSIGNMENT) {
			match(ASSIGNMENT);
			expression(/**/var_type/**/);
			printf("\tst %%reg0, %s\n", var_name);
		}

		break;
	case BEGIN:
		match(BEGIN);
		statement();

		while(lookahead == ';') {
			match(';');
			statement();
		}

		match(END);
		break;
	case IF:
		match(IF);
		var_type = expression(0);
		/**/printf("\tgofalse .L%i\n", lbl_endif = lbl_else = lbl_counter++);
		/**/if (var_type != BOOLEAN) fatal_error(INVALID_TYPE, "IF");/**/
		match(THEN);
		statement();
		if (lookahead == ELSE) {
			printf("goto .L%i\n", lbl_endif = lbl_counter++);
			printf(".L%i:\n", lbl_else);
			match(ELSE);
			statement();
		}
		printf(".L%i:\n", lbl_endif);
		break;
	case REPEAT:
		match(REPEAT);
		statement();
		while(lookahead == ';') {
			match(';');
			statement();
		}
		match(UNTIL);
		var_type = expression(0);
		/**/if (var_type != BOOLEAN) fatal_error(INVALID_TYPE, "REPEAT");/**/
		break;
	case WHILE:
		match(WHILE);
		/**/var_type = expression(0);
		if (var_type != BOOLEAN) fatal_error(INVALID_TYPE, "WHILE");/**/
		match(DO);
		statement();
		break;
	case FOR:
		match(FOR);
		match(ID);
		match(ASSIGNMENT);

		/**/var_type = expression(0);
		if (var_type != INTEGER) fatal_error(INVALID_TYPE, "FOR");/**/

		if (lookahead == TO) {
			match(TO);
		} else {
			match(DOWNTO);
		}

		/**/var_type = expression(0);
		if (var_type != INTEGER) fatal_error(INVALID_TYPE, "FOR");/**/

		match(DO);
		statement();

		break;
	}
}

int is_expression() {
	return is_simple_expression();
}

int is_simple_expression() {
	switch(lookahead) {
		case '+':
		case '-':
			return 1;
		default:
			return is_term();
	}
}

int is_term() {
	return is_factor();
}



int expression(/**/int inherited_type/**/) {
	/**/int impl_type = inherited_type,
			impl_type2,
			operator/**/;
	/**/impl_type = simple_expression(impl_type);/**/

	operator = lookahead;
	switch(lookahead) {
	case '=':
	case NEQ:
		printf("\tmov %%reg0, %%reg1\n");

		match(lookahead);
		/**/impl_type2 = simple_expression(0);/**/
		if(operator == '=') {
			printf("\tiseq %%reg1, %%reg0\t# compara e coloca em reg0 o resultado\n");
		} else {
			printf("\tisneq %%reg1, %%reg0\t# compara e coloca em reg0 o resultado\n");
		}
		return BOOLEAN;
	case '<':
	case '>':
	case LEQ:
	case GEQ:
		printf("\tmov %%reg0, %%reg1\n");

		match(lookahead);
		/**/impl_type2 = simple_expression(0);/**/
		/**/if (impl_type == BOOLEAN || impl_type2 == BOOLEAN)
		{
			fatal_error(INVALID_OPERAND, "EXPRESSION");
		}

		switch(operator) {
		case '<':
			printf("\tislo %%reg1, %%reg0\t# compara e coloca em reg0 o resultado\n");
			break;
		case '>':
			printf("\tisgr %%reg1, %%reg0\t# compara e coloca em reg0 o resultado\n");
			break;
		case LEQ:
			printf("\tisleq %%reg1, %%reg0\t#isneq compara e coloca em reg0 o resultado\n");
			break;
		case GEQ:
			printf("\tisgeq %%reg1, %%reg0\t#isneq compara e coloca em reg0 o resultado\n");
			break;
		}
		/**/
		return BOOLEAN;
	}

	return impl_type;
}

int simple_expression(/**/int inherited_type/**/) {
	/**/int impl_type = inherited_type, operation/**/;
	switch(lookahead) {
	case '+':
	case '-':
		/* simple_expression must be numeric */
		/**/impl_type = type_check(impl_type, NUMERIC_TYPE)/**/;
		/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/
		match(lookahead);
	}

	/**/impl_type = type_check(impl_type, term(impl_type));/**/
	while(lookahead == '+' || lookahead == '-' || lookahead == OR) {
		/**/impl_type = type_check(impl_type, op_type(lookahead, impl_type));/**/
		/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/

		printf("\tmov %%reg0, %%reg1\n");
		operation = lookahead;
		match(lookahead);
		/**/impl_type = type_check(impl_type, term(impl_type));/**/
		/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/

		switch(operation) {
		case '+':
			printf("\tadd %%reg1, %%reg0\n");
			break;
		case '-':
			printf("\tsub %%reg1, %%reg0\n");
			break;
		case OR:
			printf("\tor %%reg1, %%reg0\n");
			break;
		}
	}

	/**/return impl_type;/**/
}

int term(/**/int inherited_type/**/) {
	/**/int impl_type = inherited_type, operator/**/;

	/**/impl_type = type_check(impl_type, factor(impl_type))/**/;
	/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/

	while(lookahead == '*' || lookahead == '/' || lookahead == DIV ||
			lookahead == MOD || lookahead == AND) {
		operator = lookahead;

		switch(lookahead) {
		case '*':
		case '/':
		case DIV:
		case MOD:
		case AND:
			printf("\tmov %%reg0, %%reg1\n");

			/**/impl_type = type_check(impl_type, op_type(lookahead, impl_type))/**/;
			/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/
			match(lookahead);
			/**/impl_type = type_check(impl_type, factor(impl_type))/**/;
			/**/if(impl_type < 0) fatal_error(INVALID_OPERAND, NULL);/**/

			switch(operator) {
			case '*':
				printf("\tmul %%reg1, %%reg0\n");
				break;
			case '/':
				printf("\tqdiv %%reg1, %%reg0\n");
				break;
			case DIV:
				printf("\tdiv %%reg1, %%reg0\n");
				break;
			case MOD:
				printf("\tmod %%reg1, %%reg0\n");
				break;
			case AND:
				printf("\tand %%reg1, %%reg0\n");
				break;
			}
		}
	}



	return impl_type;
}

int is_factor() {
	switch(lookahead) {
	case ID:
	case '(':
	case NOT:
		return 1;
	default:
		return is_unsigned_constant();
	}
}

int factor() {
	/**/int impl_type = 0,
			sym_pos/**/;

	switch(lookahead) {
	case ID:
		if ((sym_pos = symtab_lookup(lexeme)) == -1) {
			fatal_error(SYMBOL_NOT_FOUND, lexeme);
		} else {
			impl_type = symtab_descriptor[sym_pos][1];
			printf("\tld %s, %%reg0\n", lexeme);
		}
		match(ID);
		break;
	case '(':
		match('(');
		impl_type = expression(0);
		match(')');
		break;
	case NOT:
		match(NOT);
		impl_type = factor();
		printf("\tneg %%reg0\n");
		if (impl_type != BOOLEAN) {
			fatal_error(INVALID_OPERAND, "NOT");
		}
		break;
	default:
		impl_type = unsigned_constant();
		break;
	}

	return impl_type;
}


int is_unsigned_constant() {
	switch(lookahead) {
	case FLOAT:
	case UINT:
	case TRUE:
	case FALSE:
	case ID:
	case STRCONST:
		return 1;
	default:
		return 0;
	}
}

int unsigned_constant() {
	switch(lookahead) {
	case UINT:
		printf("\tmov %s, %%reg0\n", lexeme);
		match(UINT);
		return INTEGER;
		break;
	case TRUE:
	case FALSE:
		printf("\tmov %i, %%reg0\n", lookahead == TRUE ? 1 : 0);
		match(lookahead);
		return BOOLEAN;
	default:
		printf("\tmov %s, %%reg0\n", lexeme);
		match(FLOAT);
		return REAL;
	}
}

int is_constant() {
	switch(lookahead) {
	case STRCONST:
	case '+':
	case '-':
	case ID:
	case TRUE:
	case FALSE:
		return 1;
	default:
		return is_unsigned_number();
	}
}

int is_unsigned_number() {
	switch(lookahead) {
	case UINT:
	case FLOAT:
		return 1;
	}

	return 0;
}

void unsigned_number() {
	switch(lookahead) {
	case UINT:
		match(UINT);
		break;
	default:
		match(FLOAT);
		break;
	}
}
