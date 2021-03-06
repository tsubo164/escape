/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "symbol.h"

struct parser {
  struct lexer lex;
  struct symbol_table *symtbl;
};

#define PARSER_INIT {LEXER_INIT}

extern struct ast_node *parse_file(struct parser *p, const char *filename);
extern void parse_finish(struct parser *p);

#if 0
#include <stdio.h>

struct Parser;
struct SymbolTable;
struct AstNode;

extern struct Parser *Parser_New(void);
extern void Parser_Free(struct Parser *parser);

extern struct AstNode *Parser_ParseFile(struct Parser *parser,
		FILE *input_file, struct SymbolTable *table);

extern struct AstNode *Parser_ParseString(struct Parser *parser,
		const char *input_string, struct SymbolTable *table);

struct ErrorInfo {
	int line_number;
	const char *detail;
};

extern int Parser_GetErrorCount(const struct Parser *parser);
extern int Parser_GetMaxErrorInfo(const struct Parser *parser);
extern const struct ErrorInfo *Parser_GetErrorInfo(struct Parser *parser, int index);
#endif

#endif /* XXX_H */
