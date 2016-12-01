#include "parser.h"

char filename[512], outfilenametmp[512], outfilename[512];

int main(int argc, char *argv[]){
  switch(argc){
  case 1:
    printf("\nForneça o nome arquivo pascal:\n");
    scanf(" %s", filename);
    break;
  case 2:
    strcpy(filename, argv[1]);
    break;
  }

  /* abrindo arquivo */
  strcpy(filename,"C:\\Documents and Settings\\lbi-03\\Meus documentos\\Ivo\\Compiler for pascal\\teste.pas");
  in = fopen(filename, "r+");
  if(in == NULL){
    perror(filename);
    exit(errno);
  }

  strcpy(outfilenametmp, filename);
  strcat(outfilenametmp, ".tmp");
  outtmp = fopen(outfilenametmp, "w");
  if(outtmp == NULL){
	  perror(outfilenametmp);
	  exit(errno);
  }

  inittab();	/* inicia a tabela de símbolos */
  lookahead = gettoken();
  programa();
  printtab();

  fclose(in);
  fclose(outtmp);

  outtmp = fopen(outfilenametmp, "r");
  if(outtmp == NULL){
	  perror(outfilenametmp);
	  exit(errno);
  }

  strcpy(outfilename, filename);
  strcat(outfilename, ".s");
  out = fopen(outfilename, "w");
  if(out == NULL){
	  perror(outfilename);
	  exit(errno);
  }

  semantic(PRINT_LITERAL_STATEMENTS);

  fclose(outtmp);
  remove(outfilenametmp);
  return fclose(out);
}
