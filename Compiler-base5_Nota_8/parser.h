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
**************************************************************************/


int tamanho(char tipodef[20]);
void troca(char nome1[20]);
void num_sem_sinal(void);
void cons_sem_sinal(void);
void constante(void);
void fator(void);
void termo(void);
void exp_simples(void);
void expressao(void);
void lista(void);
void comando(void);
void bloco(void);
void programa(void);
void rotinas(void);
