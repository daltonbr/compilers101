INCLUDEDIR=./
CURRENTDIR=$(PWD)
CFLAGS=-g -I$(INCLUDEDIR)

objs=		main.o\
		lexer.o\
		keywords.o\
		parser.o\
		pseudocode.o\
		symtab.o\
		typecheck.o

mypas: $(objs)
	$(CC) -o $@  $^

clean:
	$(RM) $(objs)

mostlyclean: clean
	$(RM) *~
indent:
	indent -nuts -orig -nfca -ts 3 *.[ch]
tar: mostlyclean
	(cd ..;tar zcvf MyPas_$$(date +%m%d%Y%H%M).tgz ./$$(basename $(CURRENTDIR)))
