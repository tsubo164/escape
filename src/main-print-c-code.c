/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "datatype.h"
#include "parser.h"
#include "symbol.h"
#include "lexer.h"
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_code_recursive(const struct AstNode *node, int depth);

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

	if (Parser_GetErrorCount(parser) > 0) {
		const int error_count = Parser_GetErrorCount(parser);
		const int max_error_detail = Parser_GetMaxErrorInfo(parser);
		const int N = error_count > max_error_detail ? max_error_detail : error_count;
		int i;
		for (i = 0; i < N; i++) {
			const struct ErrorInfo *info = Parser_GetErrorInfo(parser, i);
			fprintf(stderr, "*  %s: %d: %s\n", filename, info->line_number, info->detail);
		}
		if (error_count > max_error_detail) {
			fprintf(stderr, "*  %d more error(s) found\n", error_count - max_error_detail);
		}
		return -1;
	}

	printf("#include <stdio.h>\n");
	print_code_recursive(node, 0);

	if (filename != NULL) {
		fclose(file);
	}
	Parser_Free(parser);
	SymbolTable_Free(table);
	AstNode_Free(node);

	return 0;
}

typedef void (*WriteCode)(const struct AstNode *node, int depth);
struct CCode {
	int ast_node_type;
	WriteCode write_pre_code;
	WriteCode write_in_code;
	WriteCode write_post_code;
};

/* NODE_EXPR */
static void NODE_EXPR_pre_code(const struct AstNode *node, int depth)
{
	const struct Symbol *symbol = node->value.symbol;

	if (symbol == NULL) {
		printf("%g", node->value.number);
		return;
	}

	switch (symbol->type) {
	case SYM_STRING_LITERAL:
		printf("\"%s\"", symbol->name);
		break;
	}
}
static void NODE_EXPR_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_EXPR_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_FUNC_CALL */
static void NODE_FUNC_CALL_pre_code(const struct AstNode *node, int depth)
{
	const char *function_name = node->value.symbol->name;
	int i;
	for (i = 0; i < depth; i++) {
		printf("  ");
	}

	if (strcmp(function_name, "print") == 0) {
		printf("printf(");
	} else {
		printf("%s(", function_name);
	}
}
static void NODE_FUNC_CALL_in_code(const struct AstNode *node, int depth)
{
	printf(");\n");
}
static void NODE_FUNC_CALL_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_FUNC_DEF */
static void NODE_FUNC_DEF_pre_code(const struct AstNode *node, int depth)
{
	const char *function_name = node->value.symbol->name;
	const char *return_type = DataType_String(node->value.symbol->data_type);
	printf("%s %s(", return_type, function_name);
}
static void NODE_FUNC_DEF_in_code(const struct AstNode *node, int depth)
{
	printf(")\n{\n");
}
static void NODE_FUNC_DEF_post_code(const struct AstNode *node, int depth)
{
	printf("}\n");
}

/* NODE_RETURN_STMT */
static void NODE_RETURN_STMT_pre_code(const struct AstNode *node, int depth)
{
	int i;
	for (i = 0; i < depth; i++) {
		printf("  ");
	}
	printf("return ");
}
static void NODE_RETURN_STMT_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_RETURN_STMT_post_code(const struct AstNode *node, int depth)
{
	printf(";\n");
}

#define CCODE(node_type) \
	{node_type, node_type##_pre_code, node_type##_in_code, node_type##_post_code}
static const struct CCode ccodes[] = {
	CCODE(NODE_EXPR),
	CCODE(NODE_FUNC_CALL),
	CCODE(NODE_FUNC_DEF),
	CCODE(NODE_RETURN_STMT)
};
static const int N_CCODES = sizeof(ccodes)/sizeof(ccodes[0]);

static void print_code_recursive(const struct AstNode *node, int depth)
{
	const struct CCode *ccode = NULL;
	int i;

	if (node == NULL) {
		return;
	}

	for (i = 0; i < N_CCODES; i++) {
		if (ccodes[i].ast_node_type == node->type) {
			ccode = &ccodes[i];
			break;
		}
	}

	if (ccode != NULL) {
		ccode->write_pre_code(node, depth + 1);
	}

	print_code_recursive(node->left, depth);

	if (ccode != NULL) {
		ccode->write_in_code(node, depth);
	}

	print_code_recursive(node->right, depth);

	if (ccode != NULL) {
		ccode->write_post_code(node, depth);
	}
}

