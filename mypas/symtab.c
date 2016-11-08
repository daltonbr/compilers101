/**@<symtab.c>::**/

int symtab[MAX_SYMTAB_ENTRIES][2];			// onde está o tipo da variável
char symtab_stream[MAX_SYMTAB_ENTRIES*(MAXID_SIZE+1)];  // onde está o nome da variável
int symtab_stream_next_descriptor = 0;
int symtab_nextentry = 0;

#define 	VAR_ALREADY_EXISTS	-3
#define		SYMTAB_OVERFLOW		-2
#define		SUCCESS			 1
int symtab_lookup(char const *name)
{
	int i;
	for (i = symtab_nextentry-1; i > -1; i--){
		if(strcmp(symtab_stream[symtab[i][0]], name) == 0)break;
	}
	return i;
}

int symtab_append(char const *name, int type)
{
	if(symtab_lookup(name) >= 0)
		return VAR_ALREADY_EXISTS;
	else if(symtab_nextentry >= MAX_SYMTAB_ENTRIES)
		return SYMTAB_OVERFLOW;

	symtab[symtab_nextentry][0] = symtab_stream_next_descriptor;
	symtab[symtab_nextentry][1] = type;
	strcpy(symtab_stream + symtab_stream_next_descriptor, name);

	symtab_stream_next_descriptor += strlen(name) + 1;
	return symtab_nextentry++;
}