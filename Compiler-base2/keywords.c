/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>

char *keyword[] = {
  "BEGIN",
  "IF",
  "THEN",
  "ELSE",
  "WHILE",
  "DO",
  "REPEAT",
  "UNTIL",
  "OF",
  "VAR",
  "ARRAY",
  "INTEGER",
  "REAL",
  "DOUBLE",
  "CHAR",
  "STRING",
  "BOOLEAN",
  "PROGRAM",
  "PROCEDURE",
  "FUNCTION",
  "DIV",
  "MOD",
  "OR",
  "AND",
  "NOT",
  "TRUE",
  "FALSE",
  "NULL",
  "END"
};

int iskeyword(char const *identifier)
{
  int token;
  for(token=BEGIN;token<END;token++){
    if(strcmp(keyword[token-BEGIN],identifier)==0)return token;
  }
  return 0;
}







