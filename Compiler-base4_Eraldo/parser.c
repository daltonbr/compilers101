/**@<parser.c>::**/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <macros.h>
#include <parser.h>
#include <tokens.h>
#include <keywords.h>
#include <symtab.h>
#include <typecheck.h>
#include <pseudocode.h>

#define         SYNTAXERR       -1

int             fatalerror = 0;
int             lookahead;

/** Section I - CONTROL OPERATIONS **/

char            str[2];

char           *
showtoken(int token)
{
    if (token >= BEGIN && token <= END) {
        return keyword[token - BEGIN];
    }
    switch (token) {
    case ID:
        return "identifier";
    case UINT:
        return "integer constant";
    case UFLT:
        return "real constant";
    case UDBL:
        return "double real constant";
    case COLONEQ:
        return ":=";
    case STRING:
        return "string constant";
    case ASCII:
        return "single character constant";
    }
    str[1] = 0;
    str[0] = token;
    return str;
}
void
match(int expected)
{
    if (expected == lookahead) {
        lookahead = gettoken(target);
    } else {
        fprintf(stderr,
                "token mismatch. expected %s but found %s... exiting\n",
                showtoken(expected), lexeme);
        exit(SYNTAXERR);
    }
}

void
start(void)
{
    lookahead = gettoken(target);
    mypas();
    match(EOF);
}

/** Section II - MAIN BODY DEFINITION **/

/*
 * mypas is the grammar's initial symbol:
 * 
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

                                                                                           /** Section III - DECLARATIONS DEFINITION **/
/*
 * declarations -> varspecs sbpspecs 
 */
void
declarations(void)
{
    varspecs();
    sbpspecs();
}

void
varspecs(void)
{
    int             initial,
                    final;
    while (lookahead == VAR) {
        match(VAR);
        do {
            initial = symtab_nextentry;
            final = symtab_add(lexeme);
            match(ID);
            while (lookahead == ',') {
                match(',');
                final = symtab_add(lexeme);
                match(ID);
            }
            match(':');
            typespec(OBJSVAR, initial, final);
            match(';');
        } while (lookahead == ID);
    }
}

char            subprogname[MAXIDLEN + 1];
int             parmframe;
/*
 * subprogram frame is the symtab entry of the current subprogram
 */ int         subprogframe;

void
sbpspecs(void)
{
    int             isfunc = lookahead == FUNCTION;
    int             dtatype;
  _sbpcheck:
    switch (lookahead) {
    case FUNCTION:
    case PROCEDURE:
        argc = 0;
        last_parm_offset = 8;
        match(lookahead);
        strcpy(subprogname, lexeme);
        match(ID);
        parmframe = formalparm();
        subprogframe = symtab_add(subprogname) - 1;
        symtab[subprogframe].parmframe = parmframe;
        symtab[subprogframe].argc = argc;
        memcpy(symtab[subprogframe].argt, argt, argc * sizeof(int));
        if (isfunc) {
            match(':');
            dtatype = smptype();
            symtab[subprogframe].objtype = OBJFUNC;
            symtab[subprogframe].dtatype = dtatype;
        } else {
            symtab[subprogframe].objtype = OBJPROC;
        }
        match(';');
        last_local_offset = 0;
        lexlevel++;
        varspecs();
        blockstmt();
        match(';');
        lexlevel--;
        symtab_nextentry = subprogframe + 1;
        goto _sbpcheck;
    }
}

int
formalparm(void)
{
    int             frame;
    if (lookahead == '(') {
        match('(');
        frame = symtab_nextentry;
        parmpecs();
        match(')');
    }
    return frame;
}

int
parmpecs(void)
{
    int             initial,
                    final,
                    passagemethod;
  _parmentry:
    if (lookahead == VAR) {
        passagemethod = OBJRPAR;
        match(VAR);
    } else {
        passagemethod = OBJSPAR;
    }
    initial = symtab_nextentry;
    final = symtab_add(lexeme);
    match(ID);
    while (lookahead == ',') {
        match(',');
        final = symtab_add(lexeme);
        match(ID);
    }
    match(':');
    typespec(passagemethod, initial, final);
    if (lookahead == ';') {
        match(';');
        goto _parmentry;
    }
    return final - initial;
}

#define LOCAL 1
void
typespec(int objtype, int initial, int final)
{
    int             dtatype;
    /*
     * objtype = 0 => scalar variable;
     *         = 1 => scalar parameter;
     *         = 2 => reference parameter;
     *         = 3 => function;
     *         = 4 => procedure.
     */
    int             i;
    dtatype = smptype();
    for (i = initial; i < final; i++) {
        symtab[i].objtype = objtype;
        symtab[i].dtatype = dtatype;
        if (lexlevel == LOCAL) {
            int             offset =
                last_local_offset + size_of_type(dtatype);
            sprintf(symtab[i].offset, "-%i(%%ebp)", offset);
            last_local_offset = offset;
        } else
            switch (objtype) {
            case OBJSPAR:
            case OBJRPAR:
                sprintf(symtab[i].offset, "%i(%%ebp)", last_parm_offset);
                argt[argc] = dtatype;
                argc++;
                last_parm_offset += size_of_type(dtatype);
                break;
            default:
                sprintf(symtab[i].offset, "_%s", symtab[i].symbol);
            }
    }
}

/*
 * typespec -> INTEGER | BOOLEAN | REAL | DOUBLE | CHAR | STRING | ARRAY
 * '[' decimal { ',' decimal } ']' OF typespec 
 */
/*
 * 0 -> void
 * 1 -> integer
 * 2 -> real
 * 3 -> double
 * 4 -> char
 * 5 -> text
 * 6 -> boolean
 */
int
smptype(void)
{
    int             tp;
    switch (lookahead) {
    case INTEGER:
        match(lookahead);
        tp = TYPEINT;
        break;
    case REAL:
        match(lookahead);
        tp = TYPEFLT;
        break;
    case DOUBLE:
        match(lookahead);
        tp = TYPEDBL;
        break;
    case CHAR:
        match(lookahead);
        tp = TYPEASC;
        break;
    case TEXT:
        match(lookahead);
        tp = TYPESTR;
        break;
    default:
        match(BOOLEAN);
        tp = TYPELOG;
    }
    return tp;
}

int
size_of_type(int type)
{
    switch (type) {
    case INTEGER:
        return 4;
    case REAL:
        return 4;
    case DOUBLE:
        return 8;
    case CHAR:
        return 1;
    case TEXT:
        return sizeof(void *);
    default:
        return 4;
    }
}

/** Section IV - STATEMENT DEFINITIONS **/

void
stmtlist(void)
{
    stmt();
    while (lookahead == ';') {
        match(';');
        stmt();
    }
}

#define         not     !
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
        idstmt(not EXPRESSION);
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
    int             lblendif,
                    lblelse;
    match(IF);
    expression(TYPELOG);
    match(THEN);
    /**/gofalse(lblelse = lblendif = loopcount)/**/;
    loopcount++;
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        gotolabel(lblendif = loopcount);
        loopcount++;
        mklbl(lblelse);
        stmt();
    }
    mklbl(lblendif);
}

void
whilestmt(void)
{
    match(WHILE);
    expression(TYPELOG);
    match(DO);
    stmt();
}

void
repstmt(void)
{
    match(REPEAT);
    stmtlist();
    match(UNTIL);
    expression(TYPELOG);
}

int
idstmt(int context)
{
    char            objname[MAXIDLEN + 1];
    int             objentry = symtab_lookup(strcpy(objname, lexeme)),
        objtype,
        dtatype;
    if (objentry < 0) {
        fprintf(stderr, "%s not found... exiting\n", objname);
        fatalerror++;
        //exit(-9);
    }
    if (context == STATEMENT
        && (objtype = symtab[objentry].objtype) == OBJFUNC) {
        fprintf(stderr, "%s cannot exist in this context\n", objname);
        fatalerror++;
        //exit(-8);
    }
    if (context == EXPRESSION
        && (objtype = symtab[objentry].objtype) == OBJPROC) {
        fprintf(stderr, "%s cannot exist in this context\n", objname);
        //exit(ILEGCALL);
    }
    dtatype = symtab[objentry].dtatype;
    match(ID);
    if (objtype == OBJPROC || objtype == OBJFUNC) {
        if (lookahead == '(') {
            match('(');
            exprlist(symtab[objentry].argc, symtab[objentry].argt);
            match(')');
        }
    } else {
        if (lookahead == COLONEQ) {
            int             synth_type;
            match(COLONEQ);
            if (dtatype < (synth_type = expression(dtatype))) {
                fprintf(stderr, "L-type and R-type are not compatible\n");
                fatalerror++;
                //return -11;
            }
            lvalue(symtab[objentry].offset);
            return typematch(dtatype, synth_type);
        }
        rvalue(symtab[objentry].offset);
    }
    return dtatype;
}

/** Section V - EXPRESSION DEFINITION **/

/*
 * exprlist -> expr { ',' expr } 
 */
void
exprlist(int argc, int argt[])
{
    int             i = 0, exprtype;
    if (argc == 0) {
        fprintf(stderr, "no argument allowed\n");
        fatalerror++;
        //exit(-12);
    }
    exprtype = expression(argt[i]);
    push(size_of_type(exprtype));
    i++;
    while (lookahead == ',') {
        match(',');
        if (i == argc) {
            fprintf(stderr, "maximum number of arguments exceeded\n");
            fatalerror++;
            //exit(-13);
        }
        expression(argt[i]);
        i++;
    }
    if (i < argc) {
        fprintf(stderr, "missing arguments\n");
        fatalerror++;
        //exit(-14);
    }
}

int
isrelop(void)
{
    int             great,
                    less,
                    equal;
    if ((great = lookahead == '>') || (less = lookahead == '<')) {
        match(lookahead);
        if (equal = lookahead == '=') {
            match('=');
        } else if (less && (great = lookahead == '>')) {
            match('>');
        }
    } else if (lookahead == '=') {
        match('=');
        equal = 1;
    } else {
      return 0;
    }
    /*
     * 1 -> <
     * 2 -> >
     * 3 -> <>
     * 4 -> =
     * 5 -> <=
     * 6 -> >=
     */
    return less + 2 * great + 4 * equal;
}

int
expression(int inherited_type)
{
    int             synthesized_type = expr(), relop;
    if (relop = isrelop()) {
        push(size_of_type(synthesized_type));
        synthesized_type = typematch(synthesized_type, expr());
        if (synthesized_type < 0) {
            fprintf(stderr, "type mismatch in relational expression\n");
            fatalerror++;
            //exit(-7);
        }
        assemblyrelop(relop);
        synthesized_type = TYPELOG /*resulting boolean from comparison */ ;
    }
    return typematch(synthesized_type, inherited_type);
}

int
expr(void)
{
    int             synth_type = 0,
        optype;
    switch (lookahead) {
    case '-':
        synth_type = TYPEINT;
        match('-');
        break;
    case NOT:
        synth_type = TYPELOG;
        match(NOT);
    }

    if ((synth_type = typematch(synth_type, term())) < 0) {
        fprintf(stderr, "operand type mismatches unary operator\n");
        //exit(-5);
    }
    while (optype = isaddop()) {
        synth_type = typematch(synth_type, optype);
        if ((synth_type = typematch(synth_type, term())) < 0) {
            fprintf(stderr, "operand type mismatches additive operator\n");
            //exit(-5);
        }
    }
    return synth_type;
}

int
isnegate(void)
{
    return lookahead == '-' || lookahead == NOT;
}

int
isaddop(void)
{
    switch (lookahead) {
    case '+':
    case '-':
        match(lookahead);
        return 1;
    case OR:
        match(OR);
        return 4;
    }
    return 0;
}

int
ismulop(void)
{
    switch (lookahead) {
    case '*':
    case '/':
    case DIV:
    case MOD:
        match(lookahead);
        return 1;
    case AND:
        match(AND);
        return 6;
    }
    return 0;
}

int
term(void)
{
    int             synth_type = factor(),
        optype;
    while (optype = ismulop()) {
        synth_type = typematch(synth_type, optype);
        if ((synth_type = typematch(synth_type, factor())) < 0) {
            fprintf(stderr, "operand type mismatches product operator\n");
            //exit(-5);
        }
    }
    return synth_type;
}

factor(void)
{
    int             factor_type;
    switch (lookahead) {
    case ID:
        factor_type = idstmt(EXPRESSION);
        break;
    case UINT:
        factor_type = TYPEINT;
        assemblyconst(lexeme,TYPEINT);
        match(lookahead);
        break;
    case UFLT:
        factor_type = TYPEFLT;
        assemblyconst(lexeme,TYPEFLT);
        match(lookahead);
        break;
    case UDBL:
        factor_type = TYPEDBL;
        assemblyconst(lexeme,TYPEDBL);
        match(lookahead);
        break;
    case ASCII:
        factor_type = TYPEASC;
        assemblyconst(lexeme,TYPEASC);
        match(lookahead);
        break;
    case STRING:
        factor_type = TYPESTR;
        assemblyconst(lexeme,TYPESTR);
        match(lookahead);
        break;
    case TRUE:
    case FALSE:
        factor_type = TYPELOG;
        assemblyconst(lexeme,TYPELOG);
        match(lookahead);
        break;
    default:
        match('(');
        factor_type = expression(0);
        match(')');
    }
    return factor_type;
}
