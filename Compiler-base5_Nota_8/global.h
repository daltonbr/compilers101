/**************************************************************************
3§ Trabalho de compiladores
Autores: Jonni Borges Surano
	 Leonardo Mellaci de Matos
Turma:	 3§BCC Integral
Prof.Dr. Eraldo Pereira Marinho
Finalidade do programa: Fazer um compilador Pascal em linguagem C, gerando um
	programa em codigo de maquina.
Resumo: O usuario escolhe o arquivo que deseja compilar. O programa faz a compilacao
  e armazena o codigo gerado em um arquivo .obj.
Compilador usado: Turbo C++
Obs: Foi usado o padrao INTEL, e nao o AT&T, na construcao do compilador

Descrição do modulo: Esse modulo contem as declarações dos tipos de tokens definidos,
	os tipos de variaveis definidos e as variaveis globais do programa
**************************************************************************/

//tipo de token e' inteiro
#define token_t int
#define label_t int
//Definicao dos tipos de tokens definidos
#define ID 0x8000
#define CT 0x8010
#define PONTO 0x8020
#define ADD 0x8030
#define ASPA 0x8040
#define E 0x8050
#define PAR1 0x8060
#define PAR2 0x8070
#define VIRG 0x8080
#define PTPT 0x8090
#define OF 0x80A0
#define NOT 0x80B0
#define MUL 0x80C0
#define MOD 0x80D0
#define AND 0x80E0
#define OR 0x80F0
#define RELAC 0x8100
#define PROCEDURE 0x8110
#define VAR 0x8120
#define FUNCTION 0x8130
#define DPTO 0x8140
#define PTVIRG 0x8150
#define BEGIN 0x8160
#define IF 0x8170
#define THEN 0x8180
#define ELSE 0x8190
#define CASE 0x81A0
#define END 0x81B0
#define WHILE 0x81C0
#define DO 0x81D0
#define REPEAT 0x81E0
#define UNTIL 0x81F0
#define ASSIGN 0x8200
#define FOR 0x8210
#define TO 0x8220
#define GOTO 0x8230
#define CONST 0x8240
#define TYPE 0x8250
#define IGUAL 0x8260
#define IDCONST 0x8270
#define IDVAR 0x8280
#define IDTIPO 0x8390
#define IDROTINA 0x83A0
#define IDFUNC 0x83B0
#define CHAV1 0x83C0
#define CHAV2 0x83D0
#include <stdio.h>

/*define os tipos string e tabela, que é uma tabela de simbolos*/
typedef char string[20];
typedef struct{
string symbol;
string tipo;
string objtipo;
int offset;
}tabela;

extern tabela tabsimb[100];
extern FILE *buffer;
extern FILE *buffer2;
extern int lookahead;
extern int indsimb;
extern int L;
extern int argumentos;
extern int compare;
extern int princ;
extern int tipoa;
extern int indtroca;
extern int nivel;
extern string trocas[20][2];
extern string relacao;
extern string lexeme;
extern string simbolo[54];

