/**@<parser.c>::**/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <tokens.h>
#include <keywords.h>
#include <pseudocode.h>

int             lookahead;

/** MAIN BODY DEFINITION **/

/*
 * mypas -> programID declarations blockstmt '.' 
 */
void
mypas(void)
{
    programID();
    declarations();
    blockstmt();
    match('.');
}
/*
 * programID -> [ PROGRAM ID ';' ] 
 */
void
programID(void)
{
    if (lookahead == PROGRAM) {
        match(PROGRAM);
        match(ID);
        match(';');
    }
}


/** DECLARATIONS DEFINITION **/
/*
 * declarations -> varspecs sbpspecs 
 */
void
declarations(void)
{
    varspecs();
    sbpspecs();
}
/*
 * varspecs -> { VAR ID {',' ID} ':' typespec ';' {ID {',' ID} ':'
 * typespec ';'} } 
 */
void
varspecs(void)
{
    while (lookahead == VAR) {
        match(VAR);
        do {
            match(ID);
            while (lookahead == ',') {
                match(',');
                match(ID);
            }
            match(':');
            typespec();
            match(';');
        } while (lookahead == ID);
    }
}

void
parmpecs(void)
{
  _parmentry:
    if (lookahead == VAR)
        match(VAR);
    match(ID);
    while (lookahead == ',') {
        match(',');
        match(ID);
    }
    match(':');
    typespec();
    if (lookahead == ';') {
        match(';');
        goto _parmentry;
    }
}

void
formalparm(void)
{
    if (lookahead == '(') {
        match('(');
        parmpecs();
        match(')');
    }
}

void
sbpspecs(void)
{
    int             isfunc = lookahead == FUNCTION;
  _sbpcheck:
    switch (lookahead) {
    case FUNCTION:
    case PROCEDURE:
        match(lookahead);
        match(ID);
        formalparm();
        if (isfunc) {
            match(':');
            smptype();
        }
        match(';');
        declarations();
        blockstmt();
        match(';');
        goto _sbpcheck;
    }
}
/*
 * typespec -> INTEGER | BOOLEAN | REAL | DOUBLE | CHAR | STRING | ARRAY
 * '[' decimal { ',' decimal } ']' OF typespec 
 */
void
smptype(void)
{
    switch (lookahead) {
    case BOOLEAN:
    case REAL:
    case CHAR:
    case TEXT:
        lookahead = gettoken(target);
        break;
    default:
        match(INTEGER);
    }
}

void
typespec(void)
{
  _typecheck:
    if (lookahead == ARRAY) {
        match(ARRAY);
        match('[');
        match(UINT);
        while (lookahead == ',') {
            match(',');
            match(UINT);
        }
        match(']');
        match(OF);
        goto _typecheck;
    }
    smptype();
}


/** STATEMENT DEFINITIONS **/

void
stmtlist(void)
{
    stmt();
    while (lookahead == ';') {
        match(';');
        stmt();
    }
}

void
stmt(void)
{
    switch (lookahead) {
    case IF:
        ifstmt();
        break;
    case WHILE:
        whilestmt();
        break;
    case REPEAT:
        repstmt();
        break;
    case ID:
        idstmt();
        break;
    case BEGIN:
        blockstmt();
        break;
    default:
        ;
    }
}

void
blockstmt(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
}

int             lbl = 1;
void
ifstmt(void)
{
     /**/ int       lendif,
                    lelse /**/;
    match(IF);
    expression();
     /**/ gofalse(lendif = lelse = lbl);
    lbl++ /**/;
    match(THEN);
    stmt();
    if (lookahead == ELSE) {
         /**/ gotolabel(lendif = lbl);
        mklbl(lelse) /**/;
        match(ELSE);
        stmt();
    }
    /**/ mklbl(lendif) /**/;
}

void
whilestmt(void)
{
    match(WHILE);
    expression();
    match(DO);
    stmt();
}

void
repstmt(void)
{
    match(REPEAT);
    stmtlist();
    match(UNTIL);
    expression();
}

void
idstmt(void)
{
    int             isarray = 0;
    match(ID);
    isarray = (lookahead == '[');
    while (lookahead == '[') {
        match('[');
        exprlist();
        match(']');
    }
    if (lookahead == COLONEQ) {
        match(COLONEQ);
        expression();
    } else if (isarray == 0 && lookahead == '(') {
        match('(');
        exprlist();
        match(')');
    } else if (isarray) {
        match(COLONEQ);
    }
}


/** EXPRESSION DEFINITION **/


/*
 * exprlist -> expr { ',' expr } 
 */
void
exprlist(void)
{
    expression();
    while (lookahead == ',') {
        match(',');
        expression();
    }
}

int
isrelop(void)
{
    int             great,
                    less;
    if ((great = lookahead == '>') || (less = lookahead == '<')) {
        match(lookahead);
        if (lookahead == '=')
            match('=');
        else if (less && lookahead == '>')
            match('>');
        return 1;
    }
    if (lookahead == '=') {
        match('=');
        return 1;
    }
    return 0;
}

void
expression(void)
{
    expr();
    if (isrelop()) {
        expr();
    }
}

int
isnegate(void)
{
    return lookahead == '-' || lookahead == NOT;
}

int
isaddop(void)
{
     /**/ int       op = lookahead /**/;
    switch (lookahead) {
    case '+':
    case '-':
    case OR:
        match(lookahead);
        return op;
    }
    return 0;
}

int
ismulop(void)
{
     /**/ int       op = lookahead /**/;
    switch (lookahead) {
    case '*':
    case '/':
    case DIV:
    case MOD:
    case AND:
        match(lookahead);
        return op;
    }
    return 0;
}

void
term(void)
{
    factor();
    while (ismulop())
        factor();
}

void
idfactor(void)
{
     /**/ char      idname[MAXIDLEN + 1] /**/;
     /**/ strcpy(idname, lexeme) /**/;
    match(ID);
    if (lookahead == '(') {
        match('(');
        exprlist();
        match(')');
    } else {
        while (lookahead == '[') {
            match('[');
            exprlist();
            match(']');
        }
        if (lookahead == COLONEQ) {
            match(COLONEQ);
            expression();
             /**/ printf("\tstore(offset(%s),_acc_reg)\n", idname) /**/;
        }
    }
}

void
factor(void)
{
    switch (lookahead) {
    case ID:
        idfactor();
        break;
    case UINT:
    case UFLT:
    case STRING:
        match(lookahead);
        break;
    default:
        match('(');
        expression();
        match(')');
    }
}

void
expr(void)
{
     /**/ int       operator /**/;
    if (isnegate()) {
        match(lookahead);
    }
    term();
    while (operator= isaddop()) {
         /**/ printf("\tpush(_acc_reg)\n") /**/;
        term();
         /**/ do_operation(operator) /**/;
    }
}

void
match(int expected)
{
    if (expected == lookahead) {
        lookahead = gettoken(target);
    } else {
        fprintf(stderr,
                "token (%i) mismatch @ line %i - while expected %i... exiting\n",
                lookahead, lineno, expected);
        exit(-2);
    }
}

void
start(void)
{
    lookahead = gettoken(target);
    mypas();
    match(EOF);
}
