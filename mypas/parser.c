/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <lexer.h>
#include <mypas.h>
#include <pseudoassembly.h>
#include <tokens.h>
#include <parser.h>
#include <keywords.h>
#include <symtab.h>
#include <macros.h>
#include <pseudoassembly.h>

#define MAX_ARG_NUM 1024

FILE *source, *object;
int labelcounter = 1;		// global var to label in machine code

/*************************** LL(1) grammar definition ******************************
 *
 * mypas -> body .
 */
void mypas(void)
{
	body();
	match('.');
}

/*
 * body -> programhead declarative imperative
 */
void body(void)
{
	declarative();
	imperative();
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
	switch(lookahead){
		case IF:
			ifstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		case REPEAT:
			repeatstmt();
			break;
        case BEGIN:
            blockstmt();
            break;
        case WRITE:
            writestmt();
            break;
		case ID:	// hereafter we expect FIRST(expr)
		case FLT:
		case DEC:
		case TRUE:
		case FALSE:
		case NOT:
		case '-':
		case '(':
			superexpr(0);
			break;
		default:
			/*<epsilon>*/;    // Emulating empty word
	}
}

void blockstmt(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
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

/*
 * WRITE -> WRITE ( " string " )
 */
void writestmt(void) {
    match(WRITE);
    match('(');
    match(STR);
    match(')');
}
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
			stmtlist();
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
	blockstmt();
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

int vartype(void)
{
	switch(lookahead){
		case INTEGER:
			match(INTEGER);
            return INTEGER;
		case REAL:
			match(REAL);
            return REAL;
        case DOUBLE:
            match(DOUBLE);
            return DOUBLE;
		default:
			match( BOOLEAN);
            return BOOLEAN;
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

/*
 *ifstmt -> IF superexpr THEN stmtlist { ELIF superexpr THEN stmtlist } [ ELSE stmtlist ] ENDIF      // { é para fecho de klein  e [ é para opcionalidade (0 ou 1)
 */
void ifstmt(void)
{
	int syntype;
	int _endif, _else;
	match(IF);
	printf("\n: [[ifstmt]]");
//	syntype = superexpr(BOOLEAN); // TODO: check if is boolean
	if(superexpr(BOOLEAN) < 0) {
        fprintf(stderr,"incompatible unary operator: FATAL ERROR.\n");
    }
	fprintf(object,"\tjz .L%d \t [[then]] \n", _endif = _else = labelcounter++); //TODO: criar arquivo object e descomentar os outros fprintf
	_endif = _else = gofalse(labelcounter++);
	match(THEN);
	stmtlist();
	while (lookahead == ELIF) {
		match(ELIF); fprintf(object,"\t [[elif]] \n");
		syntype = superexpr(BOOLEAN);		
		match(THEN); fprintf(object,"\t [[then]] \n");
		stmtlist();
	}
	if(lookahead == ELSE) {
		match(ELSE);
		fprintf(object,"\tjmp .L %d [[endif]]\n", _endif = labelcounter++);
        fprintf(object,".L %d: \t [[else]] \n", _else);
		_endif = jump(labelcounter++);
		mklabel(_else);
		/**/
		stmtlist();
	}
	match(ENDIF);
	fprintf(object,".L %d: \t [[endif]] \n", _endif);
	mklabel(_endif);
}

/*
 * whilestmt -> ...
 *	WHILE superexpr DO blockstmt
 */
void whilestmt(void)
{
	int syntype;
	/**/int while_head, while_tail/**/;
	
	match(WHILE);
    fprintf(object, "\n[[label: while_head]]");
	/**/mklabel(while_head = labelcounter++);
	syntype = superexpr(BOOLEAN);
//	if(superexpr(BOOLEAN) < 0) // TODO: deu pau, escreve o erro tipo not boolean
	/**/gofalse(while_tail)/**/;
    match(DO);
    blockstmt();
	/**/jump(while_head = labelcounter++)/**/;
    fprintf(object, "\n[[label: while_tail]]");
    /**/mklabel(while_tail)/**/;

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

	match(DO); stmtlist(); match(WHILE);
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
    if(ltype == 0) return rtype;

	switch(ltype) {
		case BOOLEAN:
			if(rtype == ltype) {
				return ltype;
			}
			break;
		case INTEGER:
			if(rtype == ltype) {
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

int isrelop()
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
	t1 = expr(0);
	if(isrelop()) {
		t2 = expr(0);
		if(t1 != t2 || t1 == BOOLEAN || t2 == BOOLEAN) {
			return -1;
		}
        return min(BOOLEAN, t2);
	}
    if(!iscompatible(inherited_type, t1)) {
		return -1;
	}
	return max(BOOLEAN, t1);
}

int expr (int inherited_type)
{
    /*[[*/ int varlocality, lvalue_seen = 0, acctype = inherited_type, syntype, ltype, rtype; /*]]*/ 
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
					lvalue_seen = 1;
					ltype = syntype;
                    /*]]*/ 
		            match(ASGN);
					/*[[*/rtype = /*]]*/superexpr(/*[[*/ltype/*]]*/);
					/*[[*/
					if(iscompatible(ltype, rtype)){
						acctype = max(rtype,acctype);
					}else{
						acctype = -1; // sintax error
                        fprintf(stderr, "Incompatible type...fatal error.\n");
					}
					/*]]*/
				}
                else if(varlocality > -1) {  //TODO: this is really necessary
//                    fprintf(object,"\tpushl %%eax\n\tmov %s,%%eax\n",
//	                symtab_stream + symtab[varlocality][0]);

                    if( (acctype != BOOLEAN && symtab[varlocality][1] != BOOLEAN)
                        || (acctype == BOOLEAN && symtab[varlocality][1] == BOOLEAN) || acctype == 0) {
                        acctype = max(acctype, symtab[varlocality][1]);
                    }
                    else{
                        acctype = -1;
                        fprintf(stderr, "Incompatible type...fatal error.\n");
                    }
                }
				break;
			case TRUE:
                if(!iscompatible(acctype, BOOLEAN)) {
                    fprintf(stderr, "Incompatible type, expected boolean...fatal error.\n");
        			printf("LEX: %s\n", lexeme);
                }else{
                    acctype = BOOLEAN; 
                }
		        match(TRUE);
		        break;
            case FALSE:
                if(!iscompatible(acctype, BOOLEAN)) {
                    fprintf(stderr, "Incompatible type, expected boolean...fatal error.\n");
                    printf("LEX: %s\n", lexeme);
                }else{
                    acctype = BOOLEAN;
                } 
                match(FALSE);
                break;
            case DEC:
                if(acctype != BOOLEAN) {
                    acctype = max(acctype, INTEGER);
                }else{
                    fprintf(stderr, "Incompatible type, expected integer...fatal error.\n");
                    printf("LEX: %s\n", lexeme);
                }
                match(DEC);
                break;
            case FLT:
                if(acctype != BOOLEAN) {
                    acctype = max(acctype, REAL);
                }else{
                    fprintf(stderr, "Incompatible type, expected real...fatal error.\n");
                    printf("LEX: %s\n", lexeme);
                }
                {
                    float lexval= atof(lexeme);
                    char *fltIEEE = malloc(sizeof(lexeme)+2);
                    sprintf(fltIEEE, "$%i", *((int*)&lexval));
                    rmovel(fltIEEE);
                    free(fltIEEE);
                }
                match(FLT);
                break;
            case DBL:
		        if(acctype != BOOLEAN) {
			        acctype = max(acctype, DOUBLE);
		        }else{
                    fprintf(stderr, "Incompatible type, expected double...fatal error.\n");
                    printf("LEX: %s\n", lexeme);
                }
                match(DBL);
                break;
			default:
				match('(');
				/*[[*/syntype = /*]]*/superexpr(0);
				/*[[*/
                if(iscompatible(syntype, acctype)){
					acctype = max(acctype, syntype);
				} else {
					fprintf(stderr, "parenthesizes type incompatible with accumulated type");
				}
				/*]]*/
				match(')');
		}
	if(mulop()) goto F_entry;
	if(addop()) goto T_entry;

    /* expression ends down here */
//    /*[[*/ if (lvalue_seen && varlocality > -1) {
//		switch(ltype){
//			case INTEGER:case REAL:
//				lmovel(symtab_stream + symtab[varlocality][0]);
//				break;
//			case DOUBLE:
//				lmoveq(symtab_stream + symtab[varlocality][0]);
//				break;
//			default: // make case BOOLEAN:
//				;
//		}
//    }
    /*]]*/
    return acctype;  
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
