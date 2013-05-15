/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "parser.h"
#include "symbol.h"
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char **argv)
{
	const char *filename = NULL;
	FILE *file = NULL;

	struct Parser *parser = NULL;
	struct SymbolTable *table = NULL;
	struct AstNode *node = NULL;

	parser = Parser_New();
	table = SymbolTable_New();

	switch (argc) {
	case 2:
		filename = argv[1];

		file = fopen(filename, "r");
		if (file == NULL) {
			fprintf(stderr, "no input files: %s\n", filename);
			return -1;
		}
		break;

	case 1:
		file = stdin;
		break;

	default:
		fprintf(stderr, "invalid number of arguments\n");
		return -1;
	}

	node = Parser_ParseFile(parser, file, table);
	AstNode_Print(node);

	if (filename != NULL) {
		fclose(file);
	}
	Parser_Free(parser);
	SymbolTable_Free(table);
	AstNode_Free(node);

	return 0;
}

