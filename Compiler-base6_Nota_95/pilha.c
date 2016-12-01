#include <stdlib.h>
#include "pilha.h"

double pilha[256];
int topo=0;

void empilha(double valor){
	pilha[topo]=valor;
	topo++;
}

double desempilha(void){
	if (topo<=0)
		abort();
	topo--;
	return pilha[topo];
}	
		
	
