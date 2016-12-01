/**@<parser.c>::**/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <tokens.h>
#include <keywords.h>

int             lookahead;

/** MAIN BODY DEFINITION **/

void
mypas(void)
{
    programID();
    declarations();
    blockstmt();
    match('.');
}

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
 * varspecs -> { VAR ID {',' ID} ':' smptype ';' {ID {',' ID} ':'
 * smptype ';'} } 
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
            smptype();
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
    smptype();
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
     /**/ int       isfunc = lookahead == FUNCTION /**/;
  _sbpcheck:
    switch (lookahead) {
    case FUNCTION:
    case PROCEDURE:
        match(lookahead);
        match(ID);
        formalparm();
         /**/ if (isfunc) {
            match(':');
            smptype();
        }
        /**/ match(';');
        varspecs();
        blockstmt();
        match(';');
        goto _sbpcheck;
    }
}

void
smptype(void)
{
    switch (lookahead) {
    case BOOLEAN:
    case REAL:
    case DOUBLE:
    case CHAR:
    case TEXT:
        lookahead = gettoken(target);
        break;
    default:
        match(INTEGER);
    }
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

void
ifstmt(void)
{
    match(IF);
    expression();
    match(THEN);
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        stmt();
    }
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
    if (isnegate()) {
        match(lookahead);
    }
    term();
    while (operator= isaddop()) {
        term();
    }
}

/** PATTERN MATCH AND START ROUTINES **/
void
match(int expected)
{
    if (expected == lookahead) {
        lookahead = gettoken(target);
    } else {
        fprintf(stderr, "token mismatch... exiting\n");
    }
}

void
start(void)
{
    lookahead = gettoken(target);
    mypas();
    match(EOF);
}
