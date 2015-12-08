CC = cc
OPT = -O3
CFLAGS = -Wall -ansi $(OPT)
LDFLAGS = -lm
RM = rm -f

topdir      := ..
target_dir  := src
target_name := ec
library     := libesc.a
files       := \
		lexer parser stream

incdir  := $(topdir)/src
#libdir  := $(topdir)/lib
target  := $(topdir)/$(target_dir)/$(target_name)

sources := $(addsuffix .c, $(files))
objects := $(addsuffix .o, $(files))
depends := $(addsuffix .d, $(files))

.PHONY: all clean depend
all: $(target)

ec.o: ec.c
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

$(objects): %.o: %.c
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -c -o $@ $<

$(target): ec.o $(library)
	@echo '  link $(target_name)'
	@$(CC) -o $@ $^ $(LDFLAGS)

$(library): $(objects)
	@echo '  archive $^'
	@ar rc $@ $^

$(depends): %.d: %.c
	@echo '  dependency $<'
	@$(CC) $(CFLAGS) -I$(incdir) -c -MM $< > $@

clean:
	@echo '  clean $(target_name)'
	@$(RM) $(target) $(library) $(objects) $(depends) ec.o

ifneq "$(MAKECMDGOALS)" "clean"
-include $(depends)
endif
