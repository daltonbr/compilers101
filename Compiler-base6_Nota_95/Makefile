CC=gcc
CFLAGS=-Wall
teste:teste.o lexan.o parser.o symbol.o pilha.o codificador.o
symbol.o:symbol.c symbol.h
teste.o:teste.c symbol.h lexan.h parser.h
lexan.o:lexan.c lexan.h symbol.h
parser.o:parser.c parser.h lexan.h symbol.h pilha.h codificador.h
pilha.o: pilha.c pilha.h
codificador.o: codificador.c codificador.h
