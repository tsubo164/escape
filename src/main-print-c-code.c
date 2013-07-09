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

typedef struct Context {
  int depth;
} Context;

static void print_code_recursive(const struct AstNode *node, Context *cxt);

int main(int argc, const char **argv)
{
	const char *filename = NULL;
	FILE *file = NULL;

	struct Parser *parser = NULL;
	struct SymbolTable *table = NULL;
	struct AstNode *node = NULL;

  Context cxt = {0};

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
	print_code_recursive(node, &cxt);

	if (filename != NULL) {
		fclose(file);
	}
	Parser_Free(parser);
	SymbolTable_Free(table);
	AstNode_Free(node);

	return 0;
}

typedef void (*WriteCode)(const struct AstNode *node, Context *cxt);
struct CCode {
	int ast_node_op;
	WriteCode write_pre_code;
	WriteCode write_in_code;
	WriteCode write_post_code;
};

static void indent(Context *cxt)
{
	int i;
	for (i = 0; i < cxt->depth; i++) {
		printf("  ");
	}
}

/* NODE_POST_INC */
static void NODE_POST_INC_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("((");
}
static void NODE_POST_INC_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_POST_INC_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")++)");
}

/* NODE_POST_DEC */
static void NODE_POST_DEC_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("((");
}
static void NODE_POST_DEC_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_POST_DEC_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")--)");
}

/* NODE_INC */
static void NODE_INC_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_INC_in_code(const struct AstNode *node, Context *cxt)
{
	printf("++(");
}
static void NODE_INC_post_code(const struct AstNode *node, Context *cxt)
{
	printf("))");
}

/* NODE_DEC */
static void NODE_DEC_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_DEC_in_code(const struct AstNode *node, Context *cxt)
{
	printf("--(");
}
static void NODE_DEC_post_code(const struct AstNode *node, Context *cxt)
{
	printf("))");
}

/* NODE_LSHIFT */
static void NODE_LSHIFT_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_LSHIFT_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" << ");
}
static void NODE_LSHIFT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_RSHIFT */
static void NODE_RSHIFT_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_RSHIFT_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" >> ");
}
static void NODE_RSHIFT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_LT */
static void NODE_LT_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_LT_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" < ");
}
static void NODE_LT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_GT */
static void NODE_GT_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_GT_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" > ");
}
static void NODE_GT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_LE */
static void NODE_LE_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_LE_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" <= ");
}
static void NODE_LE_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_GE */
static void NODE_GE_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_GE_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" >= ");
}
static void NODE_GE_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_EQ */
static void NODE_EQ_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_EQ_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" == ");
}
static void NODE_EQ_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_NE */
static void NODE_NE_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_NE_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" != ");
}
static void NODE_NE_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_BITWISE_AND */
static void NODE_BITWISE_AND_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_BITWISE_AND_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" & ");
}
static void NODE_BITWISE_AND_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_BITWISE_XOR */
static void NODE_BITWISE_XOR_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_BITWISE_XOR_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" ^ ");
}
static void NODE_BITWISE_XOR_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_BITWISE_OR */
static void NODE_BITWISE_OR_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_BITWISE_OR_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" | ");
}
static void NODE_BITWISE_OR_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_OR */
static void NODE_OR_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_OR_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" || ");
}
static void NODE_OR_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_AND */
static void NODE_AND_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_AND_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" && ");
}
static void NODE_AND_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_ADD */
static void NODE_ADD_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_ADD_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" + ");
}
static void NODE_ADD_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_SUB */
static void NODE_SUB_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_SUB_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" - ");
}
static void NODE_SUB_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_MUL */
static void NODE_MUL_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_MUL_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" * ");
}
static void NODE_MUL_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_DIV */
static void NODE_DIV_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_DIV_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" / ");
}
static void NODE_DIV_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_MOD */
static void NODE_MOD_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_MOD_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" %% ");
}
static void NODE_MOD_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_ASSIGN */
static void NODE_ASSIGN_pre_code(const struct AstNode *node, Context *cxt)
{
	printf("(");
}
static void NODE_ASSIGN_in_code(const struct AstNode *node, Context *cxt)
{
	printf(" = ");
}
static void NODE_ASSIGN_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_CALL_EXPR */
static void NODE_CALL_EXPR_pre_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_CALL_EXPR_in_code(const struct AstNode *node, Context *cxt)
{
  printf("(");
}
static void NODE_CALL_EXPR_post_code(const struct AstNode *node, Context *cxt)
{
	printf(")");
}

/* NODE_FUNC_DEF */
static void NODE_FUNC_DEF_pre_code(const struct AstNode *node, Context *cxt)
{
	const char *function_name = node->value.symbol->name;
	const char *return_type = DataType_String(node->value.symbol->data_type);
	printf("%s %s(", return_type, function_name);
}
static void NODE_FUNC_DEF_in_code(const struct AstNode *node, Context *cxt)
{
	printf(")\n");
}
static void NODE_FUNC_DEF_post_code(const struct AstNode *node, Context *cxt)
{
}

/* NODE_EXPR_STMT */
static void NODE_EXPR_STMT_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
}
static void NODE_EXPR_STMT_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_EXPR_STMT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(";\n");
}

/* NODE_BLOCK */
static void NODE_BLOCK_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
	printf("{\n");
  cxt->depth++;
}
static void NODE_BLOCK_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_BLOCK_post_code(const struct AstNode *node, Context *cxt)
{
  cxt->depth--;
  indent(cxt);
	printf("}\n");
}

/* NODE_IF */
static void NODE_IF_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
  /* NODE_COND prints out '(' and ')' */
	printf("if");
}
static void NODE_IF_in_code(const struct AstNode *node, Context *cxt)
{
  if (node->right != NULL) {
    indent(cxt);
    printf("else\n");
  }
}
static void NODE_IF_post_code(const struct AstNode *node, Context *cxt)
{
}

/* NODE_COND */
static void NODE_COND_pre_code(const struct AstNode *node, Context *cxt)
{
	printf(" (");
}
static void NODE_COND_in_code(const struct AstNode *node, Context *cxt)
{
	printf(")\n");
  if (node->right != NULL && node->right->op != NODE_BLOCK) {
    NODE_BLOCK_pre_code(node, cxt);
  }
}
static void NODE_COND_post_code(const struct AstNode *node, Context *cxt)
{
  if (node->right != NULL && node->right->op != NODE_BLOCK) {
    NODE_BLOCK_post_code(node, cxt);
  }
}

/* NODE_WHILE */
static void NODE_WHILE_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
	printf("while (");
}
static void NODE_WHILE_in_code(const struct AstNode *node, Context *cxt)
{
	printf(")\n");
  if (node->right != NULL && node->right->op != NODE_BLOCK) {
    NODE_BLOCK_pre_code(node, cxt);
  }
}
static void NODE_WHILE_post_code(const struct AstNode *node, Context *cxt)
{
  if (node->right != NULL && node->right->op != NODE_BLOCK) {
    NODE_BLOCK_post_code(node, cxt);
  }
}

/* NODE_DO_WHILE */
static void NODE_DO_WHILE_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
	printf("do\n");
  if (node->right != NULL && node->left->op != NODE_BLOCK) {
    NODE_BLOCK_pre_code(node, cxt);
  }
}
static void NODE_DO_WHILE_in_code(const struct AstNode *node, Context *cxt)
{
  if (node->right != NULL && node->left->op != NODE_BLOCK) {
    NODE_BLOCK_post_code(node, cxt);
  }
  indent(cxt);
	printf("while (");
}
static void NODE_DO_WHILE_post_code(const struct AstNode *node, Context *cxt)
{
	printf(");\n");
}

/* NODE_RETURN_STMT */
static void NODE_RETURN_STMT_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
	printf("return ");
}
static void NODE_RETURN_STMT_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_RETURN_STMT_post_code(const struct AstNode *node, Context *cxt)
{
	printf(";\n");
}

/* NODE_SYMBOL */
static void NODE_SYMBOL_pre_code(const struct AstNode *node, Context *cxt)
{
	if (strcmp(node->value.symbol->name, "print") == 0) {
		printf("printf");
	} else {
		printf("%s", node->value.symbol->name);
	}
}
static void NODE_SYMBOL_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_SYMBOL_post_code(const struct AstNode *node, Context *cxt)
{
}

/* NODE_NUMBER_LITERAL */
static void NODE_NUMBER_LITERAL_pre_code(const struct AstNode *node, Context *cxt)
{
  printf("%g", node->value.number);
}
static void NODE_NUMBER_LITERAL_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_NUMBER_LITERAL_post_code(const struct AstNode *node, Context *cxt)
{
}

/* NODE_STRING_LITERAL */
static void NODE_STRING_LITERAL_pre_code(const struct AstNode *node, Context *cxt)
{
	const struct Symbol *symbol = node->value.symbol;
  printf("\"%s\"", symbol->name);
}
static void NODE_STRING_LITERAL_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_STRING_LITERAL_post_code(const struct AstNode *node, Context *cxt)
{
}

/* NODE_VAR_DECL */
static void NODE_VAR_DECL_pre_code(const struct AstNode *node, Context *cxt)
{
  indent(cxt);
	printf("int %s", node->value.symbol->name);
  if (node->right == NULL) {
    printf(" = 0");
  } else {
    printf(" = ");
  }
}
static void NODE_VAR_DECL_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_VAR_DECL_post_code(const struct AstNode *node, Context *cxt)
{
	printf(";\n");
}

/* NODE_VARDUMP */
static void NODE_VARDUMP_pre_code(const struct AstNode *node, Context *cxt)
{
  char c = '\0';

  switch (node->value.symbol->data_type) {
  case TYPE_INT: c = 'd'; break;
  default: break;
  }

  indent(cxt);
  printf("printf(\"#  %s => %%%c\\n\", %s)",
      node->value.symbol->name,
      c,
      node->value.symbol->name);
}
static void NODE_VARDUMP_in_code(const struct AstNode *node, Context *cxt)
{
}
static void NODE_VARDUMP_post_code(const struct AstNode *node, Context *cxt)
{
	printf(";\n");
}

#define CCODE(node_type) \
	{node_type, node_type##_pre_code, node_type##_in_code, node_type##_post_code}
static const struct CCode ccodes[] = {
	CCODE(NODE_POST_INC),
	CCODE(NODE_POST_DEC),
	CCODE(NODE_INC),
	CCODE(NODE_DEC),
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
	CCODE(NODE_MOD),
	CCODE(NODE_ASSIGN),
	CCODE(NODE_CALL_EXPR),
	CCODE(NODE_FUNC_DEF),
	CCODE(NODE_IF),
	CCODE(NODE_COND),
	CCODE(NODE_WHILE),
	CCODE(NODE_DO_WHILE),
	CCODE(NODE_EXPR_STMT),
	CCODE(NODE_BLOCK),
	CCODE(NODE_RETURN_STMT),
	CCODE(NODE_SYMBOL),
	CCODE(NODE_NUMBER_LITERAL),
	CCODE(NODE_STRING_LITERAL),
	CCODE(NODE_VAR_DECL),
	CCODE(NODE_VARDUMP)
};
static const int N_CCODES = sizeof(ccodes)/sizeof(ccodes[0]);

static void print_code_recursive(const struct AstNode *node, Context *cxt)
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
		ccode->write_pre_code(node, cxt);
	}

	print_code_recursive(node->left, cxt);

	if (ccode != NULL) {
		ccode->write_in_code(node, cxt);
	}

	print_code_recursive(node->right, cxt);

	if (ccode != NULL) {
		ccode->write_post_code(node, cxt);
	}
}

