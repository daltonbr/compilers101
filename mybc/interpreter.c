/**@<interpreter.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <string.h>
#include <interpreter.h>
#include <lexer.h>

#define	MAXSYMTAB_SIZE 	0x10000
#define MAXSTACK_SIZE 	64

int symtab_nextentry = 0;
char symtab[MAXSYMTAB_SIZE][MAXID_SIZE + 1]; //pos, nome do id
double memtab[MAXSYMTAB_SIZE]; //valor real
int sp = -1; //stack pointer
double stack[MAXSTACK_SIZE];
double accumulator;

int has_id(char *id) 
{ 	
	int index = find_id_index(id);
    if(index < 0) return 0;
    else return 1;
}

double find_id_value(char *id) 
{
    int i;
    for(i = 0; i < sizeof(symtab); i++) { 
		if(strcmp(symtab[i], id))
			return memtab[i];
	}
    return 0;
}

int find_id_index(char *id)
{
    int i;
	//if(sp == -1) return -1;

    for(i = 0; i < sizeof(memtab)/sizeof(double); i++) {
		if(strcmp(symtab[i], id) == 0) {
			return i;
		}
	}
    return -1;
}

void set_value_to_id(char *id)
{
    int index = find_id_index(id);
    if(index < 0) {
        register_id(id);
        index = find_id_index(id);
    }
    memtab[index] = accumulator;
}

void register_id(char *id)
{
    strcpy(symtab[symtab_nextentry], id);
    memtab[symtab_nextentry] = 0;
    symtab_nextentry++;
}

void add_to_stack()
{
    sp++;
    if(sp == MAXSTACK_SIZE)
        exit(1);

    stack[sp] = accumulator;
}

void clear_stack()
{
	sp = -1;
}

void clear_accumulator()
{
	accumulator = 0;
}

void next_value(double value)
{
    accumulator = value;
}

void next_value_with_id(char *id)
{
    if(!has_id(id)) {
        register_id(id);
    }
    next_value(find_id_value(id));
    
}

void operation(char op)
{
    if(sp < 0)
        exit(1);
    int first_value = stack[sp];
    int second_value = accumulator;
    sp--;
    if(op == '+')
        accumulator = first_value + second_value;
    if(op == '-')
        accumulator = first_value - second_value;
    if(op == '/')
        accumulator = first_value / second_value;
    if(op == '*')
        accumulator = first_value * second_value;

    printf("result: %.2f\n", accumulator);
} 



