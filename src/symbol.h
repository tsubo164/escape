/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include "datatype.h"

enum SymbolType {
	SYM_NONE = 0,
	SYM_CONST,
	SYM_FUNCTION,
	SYM_STRING_LITERAL,
	SYM_VAR
};

struct Symbol {
	char *name;
	int type;

	/* TODO TEST */
	DataType data_type;
};

struct SymbolTable;

extern struct SymbolTable *SymbolTable_New(void);
extern void SymbolTable_Free(struct SymbolTable *table);

extern struct Symbol *SymbolTable_Lookup(struct SymbolTable *table, const char *key);
extern struct Symbol *SymbolTable_Add(struct SymbolTable *table,
		const char *name, int type);

extern struct Symbol *SymTableLookup(
		struct SymbolTable *table,
		const char *key);

extern struct Symbol *SymTableAddFunction(
		struct SymbolTable *table,
		const char *function_name);

#endif /* XXX_H */

