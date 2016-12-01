/**@<main.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <parser.h>

FILE 		*target;

int main(int argc,char *argv[])
{
 if(argc > 1){
    target = fopen(argv[1],"r");
    if (target = NULL){
      fprintf(stderr,"cannot open target file: %s\n", argv[1]);
      exit(-1);
    }
 }else{
   target=stdin;
 }
 lookahead =gettoken(target);
 mypas();
return 0;
}