/**@<interpreter.h>::**/

#pragma once

void register_id(char *id, double value);
int find_id_index(char *id);
int has_id(char *id);
double find_id_value(char *id); 
void set_value_to_id(char *id);
void push_stack();
void pop_stack();
void clear_stack();
void clear_accumulator();
void next_value(double value);
void next_value_with_id(char *id);
void operation(char op);
