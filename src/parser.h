/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef PARSER_H
#define PARSER_H

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

#endif /* XXX_H */

