/**@<parser.c>::**/

#include <parser.h>
#include <tokens.h>
#include <keywords.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int op,lookahead;


void match(int expected)
{
  if (lookahead==expected){
  lookahead = gettoken(target);
  }
}

/* mypas -> programID declarations blockstmt '.' */
void mypas (void) 
{
  programID();
  declarations();
  blockstmt();
  match('.');
}
/* programID -> [ PROGRAM ID ';' ] */
void programID (void)
{
  if (lookahead == PROGRAM) {
    match (PROGRAM);
    match (ID); 
    match (';');
  }
}
/* declarations -> varspecs sbpspecs */
void declarations(void)
{
  varspecs(); 
  sbpspecs();
}
/* varspecs -> { VAR ID {',' ID} ':' typespec ';' } */
void varspecs(void)
{
  while (lookahead == VAR) {
    match(VAR);
    match(ID);
    while(lookahead==','){
      match(','); 
      match(ID);
    }
    match(':');
    typespec();
    match(';');
  }
}
/* typespec -> INTEGER | BOOLEAN | REAL | DOUBLE | CHAR | STRING
 *           | ARRAY '[' decimal { ',' decimal } ']' OF typespec
 */
void typespec(void)
{
  switch(lookahead){
    case INTEGER: 
    case BOOLEAN: 
    case REAL: 
    case DOUBLE: 
    case CHAR: 
    case STRING:
      lookahead = gettoken(target);
      break;
    case ARRAY:
      match(ARRAY);match('[');
      decimal(); 
      while(lookahead==',') {
        match(','); decimal();
      }
      match(']');
      match(OF); 
      typespec();
  }
}
/* sbpspecs -> FUNCTION ID [ '(' parmlist ')' ] : typespec ';' declarations blockstmt ';'
 *           | PROCEDURE ID [ '(' parmlist ')' ] ';' declarations blockstmt ';'
 **/
 void sbpspecs(void)
 {
   int isfunc;
   while((isfunc = (lookahead == FUNCTION)) || lookahead == PROCEDURE){
     match(lookahead); match(ID);
     formparm();
 }
  if(isfunc){
   match(':');builtintype();
 }
 body();
 match(';');
}

void funspec(void){
    match(FUNCTION);
    match(ID);
    formparm();
    match(':');
    builtintype();
   
    match(';');
  }
 void procedspec(void){
   match(PROCEDURE);
   match(ID);
   formparm();
  
   match(';');
} 
/*  
 * parmlist -> formparm { ';' formparm }
 **/
void parmlist(void){
formparm();
while(lookahead ==';'){
match(';');
formparm();
 }
}

/*  
 * formparm -> [ VAR ] ID {',' ID} ':' typespec
 **/
void formparm(void){
   if(lookahead == VAR) match(VAR);
   match(ID);
   while(lookahead==','){
   match(',');
   match(ID);
   }
   match(':');

}

/*  
 * 
 * blockstmt -> BEGIN stmtlist END
 **/
void blockstmt(void){
match(BEGIN);
stmtlist();
match(END);
  
}

/*  
 * stmtlist -> stmt { ';' stmt }
 **/
void stmtlist(void){
stmt();
while(lookahead==';'){
  match(';');
  stmt();
  }
}
/*  
 * stmt -> blockstmt | ifstmt | whilestmt | repstmt | idstmt
*/

void stmt(void){
  lookahead = gettoken(target);
switch(lookahead)
{
  case BEGIN:
    if(lookahead != END)
    {
    blockstmt();
    }
    break;
  case IF:
    ifstmt();
    break;
  case WHILE:
    whlstmt();
    break;
  case REPEAT:
    repstmt();
    break;
  case ID:
    idstmt();
    break;
    
   } 
}
/*
 * 
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 */

void ifstmt(void){
  match(IF);expr();match(THEN);stmt();
  if(lookahead==ELSE){
  match(ELSE);stmt();
  }
  
}
/*
 * expr -> simpexpr [ relop simpexpr ]
 */
void expr(void)
{
  simpexpr();
  while(isrelop(lookahead)){
    match(lookahead);
    smpexpr();
  }
}
/* 
 * relop = '>'['='] | '<'[ ['='|'>'] ] | '='
 */
int isrelop(void){ 
    switch(lookahead){
      case '<':
	   match(lookahead);
	   if((lookahead = gettoken(target)) == '=') {
	        match(lookahead);
	        return op;
	   }
	   break;
      case '>':
	   match(lookahead);
	   if((lookahead = gettoken(target)) == '=') {
	        match(lookahead);
	        return op;
	   }
	   break;
      case '=':
	  if((lookahead = gettoken(target)) == '='){
	        match(lookahead);
		return op;
	  }
	  return 0;
 
	  return 0;
    }
}
/*
 * simpexpr -> negate term { addop term }
 */
void simpexpr(void)
{
  if(lookahead==NOT || lookahead == '-'){
    lookahead = gettoken(target);
  }
  term();
  while(isaddop(lookahead)){
    lookahead = gettoken(target);
    term();
  }
}
/*
 * negate -> [ '-' | NOT ]
 **/
int isnegate(void) {
   return lookahead == '-' || lookahead == NOT;
}


/*  
 * addop -> '+' | '-' | OR
 **/
int isaddop(void){
  switch(lookahead) {
    case '+': case '-': case OR: match(lookahead);
    return op;
  }
  return 0;
}

/*  
 * term -> fact { mulop fact }
 */
void term(void)
{
  factor();
  while(ismulop(lookahead)) {
    match(lookahead);
    factor();
  }
}
/*
 * mulop -> '*' | '/' | DIV | MOD | AND
 **/

int ismulop(void){
  switch(lookahead){
    case '*': case '/': case DIV: case MOD: case AND: match(lookahead);
    return op;
  }
  return 0;
}
/*  
 * fact -> constant | idvalue | '(' expr ')'
 **/

void factor(void)
{
    switch(lookahead){
      case '(':
        match('('); expr(); match(')'); break;
      case UINT:case FLOAT:case TEXT:case TRUE:case FALSE:
        match(lookahead);
        break;
      default:
        match(ID);
        if(lookahead=='('){
          match('(');
          exprlist();
          match(')');
        } else {
          while(lookahead=='['){
            match('[');
            exprlist();
            match(']');
          }
          if(lookahead==ASSGM) {
            match(ASSGM); expr();
          }
        }
    }
}
/*  
 * constant -> num | string | TRUE | FALSE | NULL | char
 **/


/*  
 * char -> \'[.]\'
 **/


/*  
 * num -> decimal | fltpoint
 **/


/*  
 * decimal -> '0' | ['1'-'9']{digit}
 **/


/*  
 * fltpoint -> decimal E [ ['+'|'-'] ] digit {digit} 
 *           | (decimal [ '.' {dgigit} ] | '.'digit {digit})  [ E [ ['+'|'-'] ] digit {digit} ]
 **/


/*  
 * digit -> ['0'-'9']
 **/


/*  
 * string -> \' {[^\']} \'
 **/


/* 
 * whilestmt -> WHILE expr DO stmt
 **/
void whlstmt(void){
  match(WHILE);
  expr();
  match(DO);
  stmt();
  
}

/* 
 * repstmt -> REPEAT stmtlist UNTIL expr
 */
void repstmt(void){
  match(REPEAT);
  stmt();
  
  while(lookahead==';'){
    match(';');
    stmt();
  }
  match(UNTIL);
  expr();
}

/*
 * idstmt -> assgmnt | procedcall */
void idstmt(void)
{
  int isarray = 0;
  match(ID);
  isarray = (lookahead == '[');
  while(lookahead=='['){
    match('['); exprlist(); match(']');
  }
  if(lookahead==ASSGM){
    match(ASSGM); expr();
  } else if (isarray == 0 && lookahead == '('){
    match('('); exprlist(); match(')');
  } else if (isarray) {
    match(ASSGM);
  }
}
void idvalue(void)
{
  int isarray = 0;
  match(ID);
  isarray = (lookahead == '[');
  while(lookahead=='['){
    match('['); exprlist(); match(']');
  }
  if(lookahead==ASSGM){
    match(ASSGM); expr();
  } else if (isarray == 0 && lookahead == '('){
    match('('); exprlist(); match(')');
  }
}
/* exprlist -> expr { ',' expr } */
void exprlist(void)
{
  expr();
  while(lookahead==','){
    match(',');expr();
  }
}

/* MyPas:: My very simplified Pascal */

/* LL(1)-Grammar:
 * 
 * mypas -> programID declarations blockstmt '.'
 * 
 * programID -> [ PROGRAM ID ';' ]
 * 
 * declarations -> varspecs sbpspecs
 * 
 * varspecs -> { VAR ID {',' ID} ':' typespec ';' }
 * 
 * typespec -> INTEGER | BOOLEAN | REAL | DOUBLE | CHAR | STRING
 *           | ARRAY '[' decimal { ',' decimal } ']' of typespec
 * 
 * sbpspecs -> FUNCTION ID [ '(' parmlist ')' ] : typespec ';' declarations blockstmt ';'
 *           | PROCEDURE ID [ '(' parmlist ')' ] ';' declarations blockstmt ';'
 * 
 * parmlist -> formparm { ';' formparm }
 * 
 * formparm -> [ VAR ] ID {',' ID} ':' typespec
 * 
 * 
 * blockstmt -> BEGIN stmtlist END
 * 
 * stmtlist -> stmt { ';' stmt }
 * 
 * stmt -> blockstmt | ifstmt | whilestmt | repstmt | idstmt
 * 
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 * 
 * expr -> simpexpr [ relop simpexpr ]
 * 
 * relop = '>'['='] | '<'[ ['='|'>'] ] | '='
 * 
 * simpexpr -> negate term { addop term }
 * 
 * negate -> [ '-' | NOT ]
 * 
 * addop -> '+' | '-' | OR
 * 
 * term -> fact { mulop fact }
 * 
 * mulop -> '*' | '/' | DIV | MOD | AND
 * 
 * fact -> constant | idstmt | '(' expr ')'
 * 
 * constant -> num | string | TRUE | FALSE | NULL | char
 * 
 * char -> \'[.]\'
 * 
 * num -> decimal | fltpoint
 * 
 * decimal -> '0' | ['1'-'9']{digit}
 * 
 * fltpoint -> decimal E [ ['+'|'-'] ] digit {digit} 
 *           | (decimal [ '.' {dgigit} ] | '.'digit {digit})  [ E [ ['+'|'-'] ] digit {digit} ]
 * 
 * digit -> ['0'-'9']
 * 
 * string -> \' {[^\']} \'
 * 
 * whilestmt -> WHILE expr DO stmt
 * 
 * repstmt -> REPEAT stmtlist UNTIL expr
 * 
 * idstmt -> assgmnt | procedcall
 * 
 * assgmnt -> variable ":=" expr
 * 
 * variable -> ID { '[' exprlist ']' }
 * 
 * exprlist -> expr { ',' expr }
 * 
 * procedcall -> ID [ '(' exprlist ')' ]
 * 
 */ 

