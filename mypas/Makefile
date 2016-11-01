CFLAGS=-I. -g

project=mypas

relocatables=$(project).o lexer.o parser.o keywords.o symtab.o

executable=$(project)

default: $(executable)

$(executable): $(relocatables)
	cc -o $(executable) $(relocatables)
clean:
	$(RM)  $(relocatables)
mostlyclean: clean
	$(RM) $(executable) *~
