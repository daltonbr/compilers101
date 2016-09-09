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
char symtab[MAXSYMTAB_SIZE][MAXSIZE_LEXEME + 1];    //pos, id name
double memtab[MAXSYMTAB_SIZE];                  //float value
int sp = -1;                                    //stack pointer
double stack[MAXSTACK_SIZE];
double accumulator;

// method to check if an Identifier is already set to our memory table
int has_id(char *id) 
{ 	
	int index = find_id_index(id);
    if(index < 0) return 0;
    else return 1;
}

// method to return a value of an Identifier (already in the table)
double find_id_value(char *id) 
{
    int i;
    for(i = 0; i < sizeof(symtab); i++) { 
		if(strcmp(symtab[i], id) == 0) {
            return memtab[i];
        }
	}
    return 0; //TODO must return error?
}

// method that return the index of an identifier (in the memory table); -1 indicate ID not found
int find_id_index(char *id)
{
    int i;
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
        register_id(id, accumulator);
        index = find_id_index(id);
    }
    memtab[index] = accumulator;
}

// method to input a new ID and it's value to the memory table
void register_id(char *id, double value)
{
    strcpy(symtab[symtab_nextentry], id);
    memtab[symtab_nextentry] = value;
    symtab_nextentry++;
}

void push_stack()
{
    sp++;
    if(sp == MAXSTACK_SIZE)
        exit(1);
    stack[sp] = accumulator;
}

void pop_stack()
{
    if (sp <= -1) {
        printf("Illegal Operation: Stack Underflow!");
        exit(1);
    }
    accumulator = stack[sp];
    sp--;
}

void clear_stack()
{
	sp = -1;
}

void clear_accumulator()
{
	accumulator = 0;
}

// function to load the accumulator
void next_value(double value)
{
    accumulator = value;
}
 
//TODO check in case of updating a ID's value or auto-initializing with ZERO'
void next_value_with_id(char *id)
{
    double INITIAL_VALUE = 0.0f;
    if(!has_id(id)) {
        printf("Warning: ID not initialized! %s will be set to 0.0\n", id);
        register_id(id, INITIAL_VALUE);
    }
    next_value(find_id_value(id));  // load the id value to the accumulator
}

// method that makes an math operation between the accumulator and the stack (top) given an math operator
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