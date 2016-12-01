/**@<lexer.c>::**/
#include <stdio.h>
#include <ctype.h>
#include <lexer.h>
#include <tokens.h>

int isIDENTIFIER(FILE *target)
{
  char lexeme[1024];
  int letter = getc(target);
  int i = 0;
  if(isalpha(lexeme[i]=letter)){
    int symbol;
    i++;
    while(isalnum(lexeme[i] = symbol = getc(target))) i++;
    lexeme[i] = 0;
    ungetc(symbol,target);
    if(i=iskeyword(lexeme))return i;
    return ID;
  }
  ungetc(letter,target);
  return 0;
}

int isDECIMAL(FILE* target)
{
  /* regex: ['1'-'9']['0'-'9']* | '0' */
  int symbol = getc(target);
  if(isdigit(symbol)) {
    if(symbol=='0') {
          if(symbol=='.') {
            while(isdigit(symbol=getc(target)));
            ungetc(symbol,target);
          }
          return CTE;
    }
    while(isdigit(symbol=getc(target)));
    if(symbol=='.')
      while(isdigit(symbol=getc(target)));
    ungetc(symbol,target);
    return CTE;
  } else if(symbol=='.') {
    if(isdigit(symbol=getc(target))) {
      while(isdigit(symbol=getc(target)));
    }
  }
  ungetc(symbol,target);
  return 0;
}

int skipspaces(FILE* target)
{
  int symbol;
  while(isspace(symbol=getc(target)));
  if(symbol == EOF) return EOF;
  ungetc(symbol,target);
  return 0;
}

/** lexer-to-parser interface **/
int gettoken(FILE* target)
{
  int token;
  if(token = skipspaces(target)) return token;
  if(token = isIDENTIFIER(target)) return token;
  if(token = isDECIMAL(target)) return token;
  return getc(target);
}
