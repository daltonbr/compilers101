#include "lexer.h"

/* variáveis */
FILE *in;

/* Converte uma string para maiúscula */
void toUpperCase(char *s){
  int i, t = strlen(lexema);

  for(i = 0; i < t; i++){
    s[i] = (char)toupper(s[i]);
  }
}

/* recupera o próximo lexema e retorna um token */
int gettoken(void){
  int symbol, forward, tok;
  static int counter_labeltext = 0;
  char s[256], labelnum[256], labelchar[4], labeltext[256];

  while(TRUE){
    symbol = getc(in);
    if(isalpha(symbol)){
      ungetc(symbol,in);
      fscanf(in,"%[A-Za-z_0-9]",lexema);
      toUpperCase(lexema);
      col += strlen(lexema);
      tok = retrieve(lexema);
      if(tok) return tok;
      return insert(lexema,IDENTIFICADOR,lexema,UNDEFINED,NOTYPE);
			        /* lexema     token     offset qualificador tipo */
    }
    else if(isdigit(symbol)){
      ungetc(symbol,in);
      fscanf(in,"%[0-9]",lexema);
      forward = getc(in);
      switch(forward){
      case '.':
			/* um doble é esperado */
			strcat(lexema,".");
			forward = getc(in);
			ungetc(forward,in);
			if(!isdigit(forward)){
			  error(EXP_DOU);
			  col += strlen(lexema);
			  break;
			}
			fscanf(in,"%[0-9]",s);
			strcat(lexema,s);
			forward = getc(in);
			if(toupper(forward) != 'E'){
			  ungetc(forward,in);
			  col += strlen(lexema);
			  tok = retrieve(lexema);
			  if(tok) return tok;
           labelnum[0] = '$';
           labelnum[1] = 0;
           strcat(labelnum,lexema);
			  return insert(lexema,NUMBER,labelnum,CONSTANT,FLOAT);
           /* lexema     token     offset qualificador tipo */
			}
      case 'e': case 'E':
			strcat(lexema,"E");
			forward = getc(in);
			if(isdigit(forward)){
			  ungetc(forward,in);
			  fscanf(in,"%[0-9]",s);
			  strcat(lexema,s);
			  col += strlen(lexema);
			  tok = retrieve(lexema);
			  if(tok) return tok;
           labelnum[0] = '$';
           labelnum[1] = 0;
           strcat(labelnum,lexema);
			  return insert(lexema,NUMBER,labelnum,CONSTANT,FLOAT);
           /* lexema     token     offset qualificador tipo */
			}
			else
			  switch(forward){
			  case '-': strcat(lexema,"-");
			  case '+':
			    symbol = forward;
			    forward = getc(in);
			    if(isdigit(forward)){
			      ungetc(forward,in);
			      fscanf(in,"%[0-9]",s);
			      strcat(lexema,s);
			      col += (symbol == '+' ? strlen(lexema) + 1 : strlen(lexema));
			      tok = retrieve(lexema);
			      if(tok) return tok;
               labelnum[0] = '$';
		         labelnum[1] = 0;
      		   strcat(labelnum,lexema);
			      return insert(lexema,NUMBER,labelnum,CONSTANT,FLOAT);
               /* lexema     token     offset qualificador tipo */
			    }
			  default:
			    ungetc(forward,in);
			    error(EXP_DOU);
			    col += (symbol == '+' ? strlen(lexema) + 1 : strlen(lexema));
			  }
			break; /*case 'e': case 'E':*/
      default:
			ungetc(forward,in);
			col += strlen(lexema);
			tok = retrieve(lexema);
			if(tok) return tok;
         labelnum[0] = '$';
         labelnum[1] = 0;
         strcat(labelnum,lexema);
			return insert(lexema,NUMBER,labelnum,CONSTANT,INT);
                    /* lexema     token     offset qualificador tipo */
      }/*switch*/
    }/* if isdigit */
    else
      switch(symbol){
      case ':':
			forward = getc(in);
			if(forward == '='){
			  col += 2;
			  return ASSIGN;
			}else
			  ungetc(forward,in);
			col++;
			return symbol;
		case '<':
			forward = getc(in);
			if(forward == '='){
			  col += 2;
			  return LE;
			}else
			  ungetc(forward,in);
			col++;
			return symbol;
		case '>':
			forward = getc(in);
			if(forward == '='){
			  col += 2;
			  return GE;
			}else
			  ungetc(forward,in);
			col++;
			return symbol;
		case '.':
			forward = getc(in);
			if(forward == '.'){
			  col += 2;
			  return PTO_PTO;
			}else
			  ungetc(forward,in);
			col++;
			return symbol;
		case 39:
      	lexema[0] = (char)39;
			lexema[1] = (char)getc(in);
			forward = getc(in);
			if(forward != 39){
			  ungetc(forward, in);
			  error(EXP_APOST_SIMPLES);
			}
			lexema[2] = (char)39;
         lexema[3] = 0;
			col += strlen(lexema);
			tok = retrieve(lexema);
			if(tok) return tok;
         labelchar[0] = '$';
         labelchar[1] = 0;
         strcat(labelchar,lexema);
			return insert(lexema,LITERAL_CHAR,labelchar,CONSTANT,CHARACTER);
                    /* lexema     token     offset qualificador tipo */
		case '\"':
			tok = 0;
			forward = symbol;
			do{
			  lexema[tok++] = (char)forward;
			  lexema[tok] = 0;
			  forward = getc(in);
			}while(forward != symbol && forward != '\n');
			if(forward == symbol)
			  lexema[tok++] = (char)forward;
			else{
			  error(EXP_APOST_DUPLO);
			  line++; col = 1;
			}
			lexema[tok]=0;
			col += strlen(lexema);
			tok = retrieve(lexema);
			if(tok) return tok;
         sprintf(labeltext, ".TXT%d", counter_labeltext++);
			return insert(lexema,LITERAL_TEXT,labeltext,LITERAL,TEXT);
         /* lexema     token     offset qualificador tipo */
		case '\n':
			line++;
		   col = 1;
		   break;
		case '=':
		case '+': case '-':
		case '*': case '/':
		case '&': case '#':
		case ',': case EOF:
		case ';': case '^':
		case '(': case ')':
		case '[': case ']':
			col++;
			return symbol;
      default:
			col++;
			if(!isspace(symbol))
			  error(UNKNOWN);
		}/* switch */
  }/*while*/
}

void match(int token, action_t action, erro_t err){
	if(token == lookahead){
		if(action) semantic(action);
		lookahead = gettoken();
  }else
		error(err);
}

