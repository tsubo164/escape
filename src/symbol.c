/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "symbol.h"
#include "memory.h"
#include <string.h>
#include <assert.h>

#define HASH_SIZE 1237 /* a prime number */
#define MULTIPLIER 31

struct table_entry {
	struct symbol sym;
	struct table_entry *next;
};
#define INIT_TABLE_ENTRY {INIT_SYMBOL, NULL}

struct symbol_table {
	struct table_entry *table[HASH_SIZE];
};
#define INIT_SYBOL_TABLE {{NULL}}

static struct table_entry *new_entry(const char *name);
static void free_entry(struct table_entry *entry);
static unsigned int hash_fn(const char *key);
static char *str_dup(const char *src);

struct symbol_table *new_symbol_table(void)
{
	struct symbol_table *table = MEMORY_ALLOC(struct symbol_table);
	int i;

	if (table == NULL) {
		return NULL;
	}
	for (i = 0; i < HASH_SIZE; i++) {
		table->table[i] = NULL;
	}
	return table;
}

void free_symbol_table(struct symbol_table *table)
{
	struct table_entry *entry = NULL;
	struct table_entry *kill = NULL;
	int i;

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

struct symbol *lookup_symbol(struct symbol_table *table, const char *key)
{
	struct table_entry *entry = NULL;
	const int h = hash_fn(key);
	for (entry = table->table[h]; entry != NULL; entry = entry->next) {
		if (strcmp(key, entry->sym.name) == 0) {
      return &entry->sym;
		}
	}
	return NULL;
}

struct symbol *add_symbol(struct symbol_table *table,
		const char *name, int kind)
{
	struct table_entry *entry = NULL;
	const int h = hash_fn(name);

	for (entry = table->table[h]; entry != NULL; entry = entry->next) {
		if (strcmp(name, entry->sym.name) == 0) {
			return &entry->sym;
		}
	}
	assert(entry == NULL);

	entry = new_entry(name);
	if (entry == NULL) {
		/* TODO error handling */
		return NULL;
	}

	entry->sym.kind = kind;
	entry->sym.type = TYPE_NONE;
	entry->next = table->table[h];
	table->table[h] = entry;

	return &entry->sym;
}

static struct table_entry *new_entry(const char *name)
{
	struct table_entry *entry = MEMORY_ALLOC(struct table_entry);

	if (entry == NULL) {
		return NULL;
	}

	entry->sym.name = str_dup(name);
	if (entry->sym.name == NULL) {
		free_entry(entry);
		return NULL;
	}
	entry->sym.kind = SYM_NONE;

	return entry;
}

void free_entry(struct table_entry *entry)
{
	if (entry == NULL) {
		return;
	}
	if (entry->sym.name != NULL) {
		MEMORY_FREE(entry->sym.name);
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

const char *symbol_name(const struct symbol *sym)
{
  return sym->name;
}

int symbol_type(const struct symbol *sym)
{
  return sym->type;
}
