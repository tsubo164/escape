CC = gcc
OPT = -O3
CFLAGS = -Wall -ansi -pedantic-errors $(OPT)
LDFLAGS = -lm

RM = rm -f
INSTALL = install

library     := libesc.a

files   := ast datatype lexer parser symbol
sources := $(addsuffix .c, $(files))
objects := $(addsuffix .o, $(files))

.PHONY: all clean install depend
all: print-tree print-c-code

$(objects): %.o: %.c
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

$(library): $(objects)
	@echo '  archive $^'
	@ar rc $@ $^

#print-c-code
main-print-c-code.o: main-print-c-code.c
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

print-c-code: main-print-c-code.o $(library);
	@echo '  link $^'
	@$(CC) -o $@ $^ $(LDFLAGS)

#print-tree
main-print-tree.o: main-print-tree.c
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

print-tree: main-print-tree.o $(library);
	@echo '  link $^'
	@$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(library) $(objects)
	$(RM) main-print-tree.o print-tree
	$(RM) main-print-c-code.o print-c-code

depend:
	$(CC) $(CFLAGS) -c -MM $(sources) main-print-c-code.c main-print-tree.c

#-------------------------------------------------------------------------------
ast.o: ast.c ast.h datatype.h memory.h symbol.h
datatype.o: datatype.c datatype.h
lexer.o: lexer.c lexer.h datatype.h memory.h
parser.o: parser.c parser.h datatype.h memory.h symbol.h lexer.h ast.h
symbol.o: symbol.c symbol.h datatype.h memory.h
main-print-c-code.o: main-print-c-code.c datatype.h parser.h symbol.h \
  lexer.h ast.h
main-print-tree.o: main-print-tree.c parser.h symbol.h datatype.h ast.h
