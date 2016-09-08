char *keywords[] = {
        "if", "then", "else", "endif", "while", "do", "endwhile"
}

int iskeyword(char const *identifier)
{
	int i;
	for(i = IF; i <= ENDWHILE; i++) {
		if(strcmp(keywords[i-IF], identifier) == 0) return i;			// IF é o offset
	}

	return 0;
}
