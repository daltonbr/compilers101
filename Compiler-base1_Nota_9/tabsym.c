#include "tabsym.h"

int lookahead;
char lexema[512];

tupla_t *tab, *currentp;
int capacity = 2, size = 0;
boolean_t dobre = TRUE;

#define FUNC_PRE_DEFINED 15
#define PROC_PRE_DEFINED 6

char *reserved_words[COUNT_RESERVED_WORDS-1000]={
  "PROGRAM", "BEGIN", "END", "LABEL", "CONST", "TYPE", "VAR", "PROCEDURE",
  "FUNCTION", "IF", "THEN", "ELSE", "CASE", "OF", "WHILE", "DO", "REPEAT",
  "UNTIL", "FOR", "TO", "GOTO", "ARRAY", "FILE", "BOOLEAN", "INTEGER", "CHAR",
  "REAL", "STRING", "NOT", "OR", "DIV", "MOD"
};

/* possível remoção
char *pre_defined_functions[FUNC_PRE_DEFINED]={
  "ABS", "SQR", "ODD", "SUCC", "PRED",
  "ORD", "CHR", "TRUNC", "EOF", "SIN",
  "COS", "EXP", "LN", "SQRT", "ARCTAN"
};

char *pre_defined_procedures[PROC_PRE_DEFINED]={
  "GET", "PUT", "RESET", "REWRITE", "READ", "WRITE"
};*/


/* funções */
/** Esta função inserte na tabela um novo token.
 * @return retorna o token do novo lexema inserido.
 */
int insert(const char *s, int token, const char *offset, qualifier_t qual, type_t type){
  tupla_t *tp_aux;

  /* criando a tabela de simbolos */
  if(tab == NULL){
    tab = (tupla_t *)calloc(capacity, sizeof(tupla_t));
    if(tab == NULL){
      fprintf(stderr, "\nNão foi possível criar tabela de símbolos.");
      exit(1);
    }
  }

  /* alocando mais memória */
  if(size == capacity){
    capacity = (dobre ? capacity * 2 : capacity + 5);
    tp_aux = (tupla_t *)realloc(tab, capacity * sizeof(tupla_t));
    if(tp_aux == NULL){
      if(dobre){
      dobre = FALSE;
      capacity /= 2;
      capacity += 5;
      tp_aux = (tupla_t *)realloc(tab, capacity * sizeof(tupla_t));
      }
      if(tp_aux == NULL){
		fprintf(stderr, "Erro ao alocar memória para a tabela de símbolos.\n");
		free(tab);
		exit(1);
      }
    }
    tab = tp_aux;
  }

  /* salvando registro */
  strcpy(tab[size].lexema,s);
  tab[size].token = token;
  tab[size].offset= strdup(offset);
  tab[size].qual = qual;
  tab[size].type = type;
  currentp = &tab[size];
  size++;
  return token;
}

/**
 * Esta função elimina, caso haja, o espaço extra(capacity-size != 0) na
 * tabela de símbolos, deixando-a com o tamanho exato ao de seu conteúdo(size)
 */
void trimtab(void){
  if(size < capacity){
    capacity = size;
    tab = (tupla_t *)realloc(tab, capacity * sizeof(tupla_t));
  }
}

/**
 * Esta funções procura na tabela de símbolos o índice de um deterninado lexema.
 * @return o valor do índice para o endereçamento do vetor. Caso não encontre
 * retornará -1
 */
int findindex(const char *s){
  int j;

  for(j = 0; j < size; j++)
    if(!(strcmp(s, tab[j].lexema)))
      return j;
  return -1;
}

tupla_t *gettupla(int idx){
  if(idx < 0 || idx >= size)
	return NULL;
  else
    return &tab[idx];
}

boolean_t settupla(int idx, tupla_t tp){
  if(idx < 0 || idx >= capacity)
	return FALSE;
  else
	tab[idx] = tp;
  return TRUE;
}

void printtupla(int idx){
  if(idx < 0 || idx >= size)
    return;
  switch(tab[idx].type){
  case BOOL:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  case INT:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  case CHARACTER:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  case FLOAT:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  case TEXT:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  case NOTYPE:
    printf("\nTupla: %5d\tToken:%5d\tQualifier: %5d\tType:%5d\tOffset: %s\tLexeme: %s",
	   idx, tab[idx].token, tab[idx].qual, tab[idx].type, tab[idx].offset, tab[idx].lexema);
    break;
  }
}

/**
 * Esta função inicializa a tabela de símbolos com palavras reservadas,
 * constantes, tipos, variáveis, funções, rotinas, operadores pré-definidos. */
void inittab(void){
  int i;
  type_t tipo = BOOL;

  for(i = 0; i < COUNT_RESERVED_WORDS-1000; i++){
    if(i+1000 < BOOLEAN)
      /* inserindo palavras reservadas */
      insert(reserved_words[i],i+1000,"",RESERVED_WORD,NOTYPE);
      /* lexema     token     offset qualificador tipo */
    else if(i+1000 < NOT)
      /* inserindo tipos pré-definidos */
      insert(reserved_words[i],IDENTIFICADOR,"",ID_TIP,tipo++);
      /* lexema     token     offset qualificador tipo */
    else if(i+1000 < DIV)
      /* inserindo operadores pré-definidos */
      insert(reserved_words[i],i+1000,"",RESERVED_WORD,NOTYPE);
      /* lexema     token     offset qualificador tipo */
  }

  /* inserindo constantes pré-definidas */
  insert("TRUE",IDENTIFICADOR,"TRUE",ID_CON,BOOL);
  insert("FALSE",IDENTIFICADOR,"FALSE",ID_CON,BOOL);

  /* inserindo variáveis pré-definidas */
  /*insert("INPUT",IDENTIFICADOR,IDENTIFICADOR_VAR,TEXT);
  insert("OUTPUT",IDENTIFICADOR,IDENTIFICADOR_VAR,TEXT);

  /* inserindo funções pré-definidas */

  /* não sei o que fazer com estas funções uma vez que retornarão
     o mesmo tipo que o parâmetro
     insert("ABS",IDENTIFICADOR,IDENTIFICADOR_FUN,NONE,val);
     insert("SQR",IDENTIFICADOR,IDENTIFICADOR_FUN,NONE,val);
     insert("SUCC",IDENTIFICADOR,IDENTIFICADOR_FUN,NONE,val);
     insert("PRED",IDENTIFICADOR,IDENTIFICADOR_FUN,NONE,val);*/

  /*insert("ODD"  ,IDENTIFICADOR,IDENTIFICADOR_FUN,BOOL);
  insert("ORD"  ,IDENTIFICADOR,IDENTIFICADOR_FUN,INT);
  insert("CHR"  ,IDENTIFICADOR,IDENTIFICADOR_FUN,CHARACTER);
  insert("TRUNC",IDENTIFICADOR,IDENTIFICADOR_FUN,INT);
  /*insert("EOF",IDENTIFICADOR,IDENTIFICADOR_FUN,BOOL,val);
  insert("SIN"   ,IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);
  insert("COS"   ,IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);
  insert("ARCTAN",IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);
  insert("EXP"   ,IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);
  insert("LN"    ,IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);
  insert("SQRT"  ,IDENTIFICADOR,IDENTIFICADOR_FUN,FLOAT);*/

  /* inserindo procedimentos pré-definidos */
  insert("READ" ,IDENTIFICADOR,"",ID_PRO,NOTYPE);
  insert("WRITE",IDENTIFICADOR,"",ID_PRO,NOTYPE);
  /* não sei como fazer: get, put, reset, rewrite */
};

/**
 * Esta função procura na tabela o token de um determinado lexema.
 * @return o valor do token caso encontre ou então 0 caso não encontre.
 */
int retrieve(const char *s){
  int j;

  for(j = 0; j < size; j++)
    if(!(strcmp(s, tab[j].lexema)))
      return tab[j].token;
  return 0;
}

/**
 * Esta função imprimirá na saída padrão o conteúdo da tabela de tokens.
 */
void printtab(void){
  int i;

  printf("\n\nImprimindo o conteúdo da tabela de símbolos.\n");
  for(i = 0; i < size; i++)
    printtupla(i);
  printf("\n\nCapacidade de armazenamento: %3d",capacity);
  printf("\nQuantidade de registros:     %3d\n",size);
}

/* esta função irá retornar o índice da última tupla inserida na
 * tabela. */
int getlast(void){
  return size - 1;
}