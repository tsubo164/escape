.PHONY: build clean check dist

MAKEFLAGS := --no-print-directory
CD := cd

build:
	@$(MAKE) $(MAKEFLAGS) -C src

clean:
	@$(MAKE) $(MAKEFLAGS) -C src $@
	@$(MAKE) $(MAKEFLAGS) -C tests $@
	rm -f a.out

check: build
	@$(MAKE) $(MAKEFLAGS) -C tests $@

depend:
	@$(MAKE) -C src $@ >> src/Makefile

dist: clean
	@$(CD) ..; tar cvzf escape.tar.gz escape
