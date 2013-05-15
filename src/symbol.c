/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "symbol.h"
#include "memory.h"

#include <string.h>
#include <assert.h>

enum { HASH_SIZE = 1237 }; /* a prime number */
enum { MULTIPLIER = 31 };

struct TableEntry {
	struct Symbol symbol;
	struct TableEntry *next;
};

struct SymbolTable {
	struct TableEntry *table[HASH_SIZE];
};

static struct TableEntry *new_entry(const char *name);
static void free_entry(struct TableEntry *entry);
static unsigned int hash_fn(const char *key);
static char *str_dup(const char *src);

struct SymbolTable *SymbolTable_New(void)
{
	int i;
	struct SymbolTable *table = MEMORY_ALLOC(struct SymbolTable);

	if (table == NULL) {
		return NULL;
	}

	for (i = 0; i < HASH_SIZE; i++) {
		table->table[i] = NULL;
	}

	return table;
}

void SymbolTable_Free(struct SymbolTable *table)
{
	int i;
	struct TableEntry *entry = NULL;
	struct TableEntry *kill = NULL;

	if (table == NULL) {
		return;
	}

	for (i = 0; i < HASH_SIZE; i++) {
		if (table->table[i] == NULL) {
			continue;
		}
		for (entry = table->table[i]; entry != NULL; ) {
			kill = entry;
			entry = entry->next;
			free_entry(kill);
		}
	}

	MEMORY_FREE(table);
}

struct Symbol *SymbolTable_Lookup(struct SymbolTable *table, const char *key)
{
	struct TableEntry *entry = NULL;
	const int h = hash_fn(key);

	for (entry = table->table[h]; entry != NULL; entry = entry->next) {
		if (strcmp(key, entry->symbol.name) == 0) {
			break;
		}
	}

	return &entry->symbol;
}

struct Symbol *SymbolTable_Add(struct SymbolTable *table,
		const char *name, int type)
{
	struct TableEntry *entry = NULL;
	const int h = hash_fn(name);

	for (entry = table->table[h]; entry != NULL; entry = entry->next) {
		if (strcmp(name, entry->symbol.name) == 0) {
			return &entry->symbol;
		}
	}
	assert(entry == NULL);

	entry = new_entry(name);
	if (entry == NULL) {
		/* TODO error handling */
		return NULL;
	}

	entry->symbol.type = type;
	entry->symbol.data_type = TYPE_NONE;
	entry->next = table->table[h];
	table->table[h] = entry;

	return &entry->symbol;
}

static struct TableEntry *new_entry(const char *name)
{
	struct TableEntry *entry = MEMORY_ALLOC(struct TableEntry);

	if (entry == NULL) {
		return NULL;
	}

	entry->symbol.name = str_dup(name);
	if (entry->symbol.name == NULL) {
		free_entry(entry);
		return NULL;
	}
	entry->symbol.type = SYM_NONE;

	return entry;
}

void free_entry(struct TableEntry *entry)
{
	if (entry == NULL) {
		return;
	}
	if (entry->symbol.name != NULL) {
		MEMORY_FREE(entry->symbol.name);
	}
	MEMORY_FREE(entry);
}

static unsigned int hash_fn(const char *key)
{
	unsigned int h = 0;
	unsigned char *p = NULL;

	for (p = (unsigned char *) key; *p != '\0'; p++) {
		h = MULTIPLIER * h + *p;
	}

	return h % HASH_SIZE;
}

static char *str_dup(const char *src)
{
	size_t alloc_size = 0;
	char *dst = 0;

	if (src == NULL) {
		return NULL;
	}

	alloc_size = strlen(src) + 1;

	dst = MEMORY_ALLOC_ARRAY(char, alloc_size);
	if (dst == NULL) {
		return NULL;
	}

	strncpy(dst, src, alloc_size);
	return dst;
}

