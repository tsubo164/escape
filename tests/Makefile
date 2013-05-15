CC = gcc
OPT = -O3
CFLAGS = -I../src -Wall -ansi -pedantic-errors $(OPT)
LDFLAGS = -L../src -lesc

RM = rm -f

files   := lexer_test
sources := $(addsuffix .c, $(files))
objects := $(addsuffix .o, $(files))
targets := $(files)
test_object := Test.o

.PHONY: all clean
all: $(targets)

$(test_object): Test.c Test.h
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

$(objects): %.o: %.c Test.h ../src/libesc.a
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

$(targets): %: %.o $(test_object)
	@echo '  linking $<'
	@$(CC) -o $@ $< $(test_object) $(LDFLAGS)

check: $(targets)
	@for t in $(targets); \
	do echo running :$$t; ./$$t; \
	done;

clean:
	$(RM) $(targets) $(objects) $(test_object)
