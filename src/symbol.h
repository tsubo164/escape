/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

enum symbol_kind {
	SYM_NONE = 0,
	SYM_VAR,
	SYM_CONST,
	SYM_FUNCTION,
	SYM_LABEL,
	SYM_STRING_LITERAL
};

struct symbol {
	char *name;
	int kind;
  int type;
};
#define INIT_SYMBOL {NULL, 0, 0};

extern struct symbol_table *new_symbol_table(void);
extern void free_symbol_table(struct symbol_table *table);

extern struct symbol *lookup_symbol(struct symbol_table *table, const char *key);
extern struct symbol *add_symbol(struct symbol_table *table,
		const char *name, int kind);

#endif /* XXX_H */
