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
	int ast_node_op;
	WriteCode write_pre_code;
	WriteCode write_in_code;
	WriteCode write_post_code;
};

static void indent(int depth)
{
	int i;
	for (i = 0; i < depth; i++) {
		printf("  ");
	}
}

/* NODE_LSHIFT */
static void NODE_LSHIFT_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_LSHIFT_in_code(const struct AstNode *node, int depth)
{
	printf(" << ");
}
static void NODE_LSHIFT_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_RSHIFT */
static void NODE_RSHIFT_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_RSHIFT_in_code(const struct AstNode *node, int depth)
{
	printf(" >> ");
}
static void NODE_RSHIFT_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_LT */
static void NODE_LT_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_LT_in_code(const struct AstNode *node, int depth)
{
	printf(" < ");
}
static void NODE_LT_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_GT */
static void NODE_GT_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_GT_in_code(const struct AstNode *node, int depth)
{
	printf(" > ");
}
static void NODE_GT_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_LE */
static void NODE_LE_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_LE_in_code(const struct AstNode *node, int depth)
{
	printf(" <= ");
}
static void NODE_LE_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_GE */
static void NODE_GE_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_GE_in_code(const struct AstNode *node, int depth)
{
	printf(" >= ");
}
static void NODE_GE_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_EQ */
static void NODE_EQ_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_EQ_in_code(const struct AstNode *node, int depth)
{
	printf(" == ");
}
static void NODE_EQ_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_NE */
static void NODE_NE_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_NE_in_code(const struct AstNode *node, int depth)
{
	printf(" != ");
}
static void NODE_NE_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_BITWISE_AND */
static void NODE_BITWISE_AND_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_BITWISE_AND_in_code(const struct AstNode *node, int depth)
{
	printf(" & ");
}
static void NODE_BITWISE_AND_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_BITWISE_XOR */
static void NODE_BITWISE_XOR_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_BITWISE_XOR_in_code(const struct AstNode *node, int depth)
{
	printf(" ^ ");
}
static void NODE_BITWISE_XOR_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_BITWISE_OR */
static void NODE_BITWISE_OR_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_BITWISE_OR_in_code(const struct AstNode *node, int depth)
{
	printf(" | ");
}
static void NODE_BITWISE_OR_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_OR */
static void NODE_OR_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_OR_in_code(const struct AstNode *node, int depth)
{
	printf(" || ");
}
static void NODE_OR_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_AND */
static void NODE_AND_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_AND_in_code(const struct AstNode *node, int depth)
{
	printf(" && ");
}
static void NODE_AND_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_ADD */
static void NODE_ADD_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_ADD_in_code(const struct AstNode *node, int depth)
{
	printf(" + ");
}
static void NODE_ADD_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_SUB */
static void NODE_SUB_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_SUB_in_code(const struct AstNode *node, int depth)
{
	printf(" - ");
}
static void NODE_SUB_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_MUL */
static void NODE_MUL_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_MUL_in_code(const struct AstNode *node, int depth)
{
	printf(" * ");
}
static void NODE_MUL_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_DIV */
static void NODE_DIV_pre_code(const struct AstNode *node, int depth)
{
	printf("(");
}
static void NODE_DIV_in_code(const struct AstNode *node, int depth)
{
	printf(" / ");
}
static void NODE_DIV_post_code(const struct AstNode *node, int depth)
{
	printf(")");
}

/* NODE_ASSIGN */
static void NODE_ASSIGN_pre_code(const struct AstNode *node, int depth)
{
  indent(depth);
	printf("%s = ", node->value.symbol->name);
}
static void NODE_ASSIGN_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_ASSIGN_post_code(const struct AstNode *node, int depth)
{
	printf(";\n");
}

/* NODE_EXPR */
static void NODE_EXPR_pre_code(const struct AstNode *node, int depth)
{
  printf("%g", node->value.number);
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

  indent(depth);

	if (strcmp(function_name, "print") == 0) {
		printf("printf(");
	} else {
		printf("%s(", function_name);
	}
}
static void NODE_FUNC_CALL_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_FUNC_CALL_post_code(const struct AstNode *node, int depth)
{
	printf(");\n");
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

/* NODE_BLOCK */
static void NODE_BLOCK_pre_code(const struct AstNode *node, int depth)
{
	printf("{\n");
}
static void NODE_BLOCK_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_BLOCK_post_code(const struct AstNode *node, int depth)
{
	printf("}\n");
}

/* NODE_IF */
static void NODE_IF_pre_code(const struct AstNode *node, int depth)
{
  indent(depth);
	printf("if");
}
static void NODE_IF_in_code(const struct AstNode *node, int depth)
{
	printf(" else {");
}
static void NODE_IF_post_code(const struct AstNode *node, int depth)
{
	printf("}\n");
}

/* NODE_COND */
static void NODE_COND_pre_code(const struct AstNode *node, int depth)
{
	printf(" (");
}
static void NODE_COND_in_code(const struct AstNode *node, int depth)
{
	printf(") {");
}
static void NODE_COND_post_code(const struct AstNode *node, int depth)
{
	printf("}\n");
}

/* NODE_WHILE */
static void NODE_WHILE_pre_code(const struct AstNode *node, int depth)
{
  indent(depth);
	printf("while (");
}
static void NODE_WHILE_in_code(const struct AstNode *node, int depth)
{
	printf(") \n");
}
static void NODE_WHILE_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_RETURN_STMT */
static void NODE_RETURN_STMT_pre_code(const struct AstNode *node, int depth)
{
  indent(depth);
	printf("return ");
}
static void NODE_RETURN_STMT_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_RETURN_STMT_post_code(const struct AstNode *node, int depth)
{
	printf(";\n");
}

/* NODE_SYMBOL */
static void NODE_SYMBOL_pre_code(const struct AstNode *node, int depth)
{
  printf("%s", node->value.symbol->name);
}
static void NODE_SYMBOL_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_SYMBOL_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_NUMBER_LITERAL */
static void NODE_NUMBER_LITERAL_pre_code(const struct AstNode *node, int depth)
{
  printf("%g", node->value.number);
}
static void NODE_NUMBER_LITERAL_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_NUMBER_LITERAL_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_STRING_LITERAL */
static void NODE_STRING_LITERAL_pre_code(const struct AstNode *node, int depth)
{
	const struct Symbol *symbol = node->value.symbol;
  printf("\"%s\"", symbol->name);
}
static void NODE_STRING_LITERAL_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_STRING_LITERAL_post_code(const struct AstNode *node, int depth)
{
}

/* NODE_VAR_DECL */
static void NODE_VAR_DECL_pre_code(const struct AstNode *node, int depth)
{
  indent(depth);
	printf("int %s", node->value.symbol->name);
  if (node->right == NULL) {
    printf(" = 0");
  } else {
    printf(" = ");
  }
}
static void NODE_VAR_DECL_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_VAR_DECL_post_code(const struct AstNode *node, int depth)
{
	printf(";\n");
}

/* NODE_VARDUMP */
static void NODE_VARDUMP_pre_code(const struct AstNode *node, int depth)
{
  char c = '\0';
  indent(depth);

  switch (node->value.symbol->data_type) {
  case TYPE_INT: c = 'd'; break;
  default: break;
  }

  printf("printf(\"#  %s => %%%c\\n\", %s)",
      node->value.symbol->name,
      c,
      node->value.symbol->name);
}
static void NODE_VARDUMP_in_code(const struct AstNode *node, int depth)
{
}
static void NODE_VARDUMP_post_code(const struct AstNode *node, int depth)
{
	printf(";\n");
}

#define CCODE(node_type) \
	{node_type, node_type##_pre_code, node_type##_in_code, node_type##_post_code}
static const struct CCode ccodes[] = {
	CCODE(NODE_LSHIFT),
	CCODE(NODE_RSHIFT),
	CCODE(NODE_LT),
	CCODE(NODE_GT),
	CCODE(NODE_LE),
	CCODE(NODE_GE),
	CCODE(NODE_EQ),
	CCODE(NODE_NE),
	CCODE(NODE_BITWISE_AND),
	CCODE(NODE_BITWISE_XOR),
	CCODE(NODE_BITWISE_OR),
	CCODE(NODE_OR),
	CCODE(NODE_AND),
	CCODE(NODE_ADD),
	CCODE(NODE_SUB),
	CCODE(NODE_MUL),
	CCODE(NODE_DIV),
	CCODE(NODE_ASSIGN),
	CCODE(NODE_EXPR),
	CCODE(NODE_FUNC_CALL),
	CCODE(NODE_FUNC_DEF),
	CCODE(NODE_IF),
	CCODE(NODE_COND),
	CCODE(NODE_WHILE),
	CCODE(NODE_BLOCK),
	CCODE(NODE_RETURN_STMT),
	CCODE(NODE_SYMBOL),
	CCODE(NODE_NUMBER_LITERAL),
	CCODE(NODE_STRING_LITERAL),
	CCODE(NODE_VAR_DECL),
	CCODE(NODE_VARDUMP)
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
		if (ccodes[i].ast_node_op == node->op) {
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

