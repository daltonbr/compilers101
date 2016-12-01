/**************************************************************************
3� Trabalho de compiladores
Autores: Jonni Borges Surano
	 Leonardo Mellaci de Matos
Turma:	 3�BCC Integral
Prof.Dr. Eraldo Pereira Marinho
Finalidade do programa: Fazer um compilador Pascal em linguagem C, gerando um
	programa em codigo de maquina.
Resumo: O usuario escolhe o arquivo que deseja compilar. O programa faz a compilacao
  e armazena o codigo gerado em um arquivo .obj.
Compilador usado: Turbo C++
Obs: Foi usado o padrao INTEL, e nao o AT&T, na construcao do compilador

Descri��o do modulo: Este � o modulo principal do programa, onde � feita a leitura dos
	arquivos de entrada e saida e a chamada dos analisadores sintatico e lexico

**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "main.h"
#include "parser.h"
#include "global.h"

/******************Variaveis globais*****************************************************
Tabela ? tipo que define uma tabela de s�mbolos. � definido por 4 campos: ?symbol?, ?tipo?, ?objtipo? e ?offset?;
tabsimb[100] ? tabela de s�mbolos com um m�ximo de 100 posi��es;
buffer ? define o arquivo a ser compilado. Esse arquivo ser� do tipo ?*.pas?;
buffer2 ? define o arquivo que receber� o c�digo objeto. Esse arquivo ser� do tipo ?*.obj?;
lookahead ? indica qual � o pr�ximo token;
indsimb ? representa a �ltima posi��o usada na tabela de s�mbolo. Inicialmente �   ??1?, pois n�o h� elementos na tabela;
L ? representa o pr�ximo r�tulo a ser definido. Inicialmente � ?0?, ou seja, o pr�ximo r�tulo � ?0?;
Argumentos ? armazena o n�mero de argumentos da defini��o de uma fun��o ou procedimento;
Compare ? vari�vel auxiliar usada no procedimento express�o. Se ela for ?0?, n�o deve haver compara��o, ou seja, n�o deve-se escrever no buffer de sa�da um desvio condicional, ao fim do procedimento express�o; caso contr�rio deve haver compara��o
Princ ? vari�vel auxiliar que indica se a an�lise que est� sendo feita pertence ao programa principal ou n�o. Se for ?1?, quer dizer que o compilador est� no programa principal; caso contr�rio, n�o est�;
Indtroca - representa a �ltima posi��o usada na tabela de trocas. Inicialmente �     ??1?, pois n�o h� elementos na tabela;
N�vel ? n�vel l�xico atual do programa. Inicialmente � zero;
trocas[20][2] ? tabela que armazena as vari�veis que devem ter seu nome trocado pelo seu offset em rela��o � pilha,que � do tipo ?8(%ebp)?;
lexeme ? guarda o valor do �ltimo lexeme lido pelo analisador l�xico;
simbolo[54] ? armazena as palavras reservadas em pascal.
*****************fim da declara��o de variaveis globais************************************/
tabela tabsimb[100];
FILE *buffer;
FILE *buffer2;
int lookahead;
int indsimb=-1;
int L=0;
int argumentos;
int compare;
int princ=1;
int tipoa=-1;
int indtroca=-1;
int nivel=0;
string trocas[20][2];
string relacao;
string lexeme;
string simbolo[54];
extern void programa(void);


int main(void)
{
/*nomeArq e' o nome do arquivo a ser compilado*/
string nomeArq;
int x;
/*clrscr();*/
/*defini��o da tabela de simbolos pre definidos*/
strcpy(simbolo[0],"begin");
strcpy(simbolo[1],"E");
strcpy(simbolo[2],"end");
strcpy(simbolo[3],"mod");
strcpy(simbolo[4],"and");
strcpy(simbolo[5],"or");
strcpy(simbolo[6],"not");
strcpy(simbolo[7],"if");
strcpy(simbolo[8],"then");
strcpy(simbolo[9],"else");
strcpy(simbolo[10],"case");
strcpy(simbolo[11],"of");
strcpy(simbolo[12],"while");
strcpy(simbolo[13],"do");
strcpy(simbolo[14],"repeat");
strcpy(simbolo[15],"until");
strcpy(simbolo[16],"for");
strcpy(simbolo[17],"to");
strcpy(simbolo[18],"const");
strcpy(simbolo[19],"type");
strcpy(simbolo[20],"var");
strcpy(simbolo[21],"procedure");
strcpy(simbolo[22],"function");
strcpy(simbolo[23],"goto");
strcpy(simbolo[24],"false");
strcpy(simbolo[25],"true");
strcpy(simbolo[26],"boolean");
strcpy(simbolo[27],"integer");
strcpy(simbolo[28],"char");
strcpy(simbolo[29],"real");
strcpy(simbolo[30],"text");
strcpy(simbolo[31],"input");
strcpy(simbolo[32],"output");
strcpy(simbolo[33],"get");
strcpy(simbolo[34],"put");
strcpy(simbolo[35],"reset");
strcpy(simbolo[36],"rewrite");
strcpy(simbolo[37],"read");
strcpy(simbolo[38],"write");
strcpy(simbolo[39],"abs");
strcpy(simbolo[40],"sqr");
strcpy(simbolo[41],"odd");
strcpy(simbolo[42],"succ");
strcpy(simbolo[43],"pred");
strcpy(simbolo[44],"ord");
strcpy(simbolo[45],"chr");
strcpy(simbolo[46],"trunc");
strcpy(simbolo[47],"eof");
strcpy(simbolo[48],"sin");
strcpy(simbolo[49],"cos");
strcpy(simbolo[50],"exp");
strcpy(simbolo[51],"ln");
strcpy(simbolo[52],"sqrt");
strcpy(simbolo[53],"arctan");

/*escolha do arquivo que se deseja compilar*/
printf("Digite o nome do arquivo que deseja compilar:\n");
scanf("%s",&nomeArq);
x=strlen(nomeArq);
if(x>3){
//verifica se o tipo do arquivo esta definido
	if((nomeArq[x-4])!='.'){strcat(nomeArq,".pas");}}
else   /*converte o arquivo sem tipo definido em .pas*/
	strcat(nomeArq,".pas");
buffer=fopen(nomeArq,"r");
x=strlen(nomeArq);
nomeArq[x-3]='o';
nomeArq[x-2]='b';
nomeArq[x-1]='j';
/*arquivo de saida e'do tipo obj*/
buffer2=fopen(nomeArq,"w");
lookahead=gettoken();
programa();
rotinas();
/*mensagem que indica o sucesso da compila��o*/
printf("O programa esta correto");
fclose(buffer);
fclose(buffer2);
/*getch();*/
}

