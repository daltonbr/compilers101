# Project MyPas
#
CFLAGS=-g -I. 

mypas: Main.o Lexer.o Keywords.o Parser.o ErrorHandler.o Symtab.o
	$(CC) -o $@ $^
clean:
	$(RM) *.o
mostlyclean: clean
	$(RM) *~
