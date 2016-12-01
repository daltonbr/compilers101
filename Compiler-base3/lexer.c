/**@<lexer.c>::**/
#include <stdio.h>
#include <ctype.h>
#include <lexer.h>
#include <tokens.h>
#include <keywords.h>

char lexeme[MAXIDLEN + 1];
int             lineno = 1;

int isIDENTIFIER(FILE *target)
{
  
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

int
isNUM(FILE * target)
{
    int             i = 0;
    int             token = 0;
    if (isdigit(lexeme[i] = getc(target))) {
        token = UINT;
        if (lexeme[i] == 0) {
            i++;
            if ((lexeme[i] = getc(target)) == '.') {
                token = UFLT;
            }
        } else {
            for (i = 1; isdigit(lexeme[i] = getc(target)); i++);
            if (lexeme[i] == '.') {
                token = UFLT;
            }
        }
    } else if (lexeme[i] == '.') {
        i++;
        if (isdigit(lexeme[i] = getc(target))) {
            token = UFLT;
        } else {
            ungetc(lexeme[i], target);
            i--;
        }
    }
    if (token == UFLT) {
        for (i++; isdigit(lexeme[i] = getc(target)); i++);
    }
    if (token) {
        int             j = i;
        if (toupper(lexeme[i]) == 'E') {
            i++;
            lexeme[i] = getc(target);
            if (lexeme[i] == '+' || lexeme[i] == '-') {
                i++;
                lexeme[i] = getc(target);
            }
            if (isdigit(lexeme[i])) {
                token = UFLT;
                for (i++; isdigit(lexeme[i] = getc(target)); i++);
            } else {
                for (; i > j; i--) {
                    ungetc(lexeme[i], target);
                }
            }
        }
    }
    if (token == UFLT && i > 0x10)
        token = UDBL;
    ungetc(lexeme[i], target);
    lexeme[i] = 0;
    return token;
}
int
isCOLONEQ(FILE * target)
{
    if ((lexeme[0] = getc(target)) == ':') {
        if ((lexeme[1] = getc(target)) == '=') {
            lexeme[2] = 0;
            return COLONEQ;
        }
        ungetc(lexeme[1], target);
    }
    ungetc(lexeme[0], target);
    return lexeme[0] = 0;
}
int skipspaces(FILE* target)
{

    int             symbol;
  _recheck:
    while (isspace(symbol = getc(target)))
        if (symbol == '\n')
            lineno++;
    if (symbol == EOF)
        return EOF;
    if (symbol == '{') {
        while ((symbol = getc(target)) != '}') {
            while (symbol == '\\') {
                symbol = getc(target);
                if (symbol == '\n')
                    lineno++;
            }
            if (symbol == '\n')
                lineno++;
            if (symbol == EOF)
                return EOF;
        }
        goto _recheck;
    }
    ungetc(symbol, target);
    return 0;
}

int
isSTRING(FILE * target)
{
    if ((lexeme[0] = getc(target)) == '\'') {
        int             i = 1;
        while ((lexeme[i] = getc(target)) != '\'') {
            (lexeme[i] == '\n') && lineno++;
            if (lexeme[i] == EOF)
                return EOF;
            i++;
        }
        lexeme[i + 1] = 0;
        if (i > 2)
            return STRING;
        return ASCII;
    }
    ungetc(lexeme[0], target);
    lexeme[0] = 0;
    return 0;
}

/** lexer-to-parser interface **/
int gettoken(FILE* target)
{
  int token;
  if(token = skipspaces(target)) return token;
  if(token = isIDENTIFIER(target)) return token;
  if(token = isNUM(target)) return token;
  if(token = isCOLONEQ(target)) return token;
  if(token = isSTRING(target)) return token;
  return getc(target);
}
