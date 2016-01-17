/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "cgen.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct context context_t;
typedef struct ast_node node_t;

static void print_code_recursive(FILE *fp, const node_t *node, context_t *cxt);

void print_c_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, "#include <stdio.h>\n");
  print_code_recursive(fp, node, cxt);
}

/*
  printf("#define VARDUMP(var,type,spec) printf(\"#  %%s => %%\"#spec\" (%%s)\\n\", #var, var, #type)\n");
int main(int argc, const char **argv)
{
	const char *filename = NULL;
	FILE *file = NULL;

	struct Parser *parser = NULL;
	struct SymbolTable *table = NULL;
	node_t *node = NULL;

  context_t cxt = {0};

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
*/

static void indent(FILE *fp, context_t *cxt)
{
	int i;
	for (i = 0; i < cxt->depth; i++) {
		fprintf(fp, "  ");
	}
}

/* AST_POST_INC */
static void AST_POST_INC_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "((");
}
static void AST_POST_INC_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_POST_INC_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")++)");
}

/* AST_POST_DEC */
static void AST_POST_DEC_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "((");
}
static void AST_POST_DEC_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_POST_DEC_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")--)");
}

/* AST_PRE_INC */
static void AST_PRE_INC_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_PRE_INC_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "++(");
}
static void AST_PRE_INC_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "))");
}

/* AST_PRE_DEC */
static void AST_PRE_DEC_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_PRE_DEC_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "--(");
}
static void AST_PRE_DEC_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "))");
}

/* AST_LSHIFT */
static void AST_LSHIFT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_LSHIFT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " << ");
}
static void AST_LSHIFT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_RSHIFT */
static void AST_RSHIFT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_RSHIFT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " >> ");
}
static void AST_RSHIFT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_LT */
static void AST_LT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_LT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " < ");
}
static void AST_LT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_GT */
static void AST_GT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_GT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " > ");
}
static void AST_GT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_LE */
static void AST_LE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_LE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " <= ");
}
static void AST_LE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_GE */
static void AST_GE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_GE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " >= ");
}
static void AST_GE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_EQ */
static void AST_EQ_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, "(");
}
static void AST_EQ_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " == ");
}
static void AST_EQ_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, ")");
}

/* AST_NE */
static void AST_NE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_NE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " != ");
}
static void AST_NE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_BITWISE_AND */
static void AST_BITWISE_AND_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_BITWISE_AND_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " & ");
}
static void AST_BITWISE_AND_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_BITWISE_XOR */
static void AST_BITWISE_XOR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_BITWISE_XOR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " ^ ");
}
static void AST_BITWISE_XOR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_BITWISE_OR */
static void AST_BITWISE_OR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_BITWISE_OR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " | ");
}
static void AST_BITWISE_OR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_OR */
static void AST_OR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_OR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " || ");
}
static void AST_OR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_AND */
static void AST_AND_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_AND_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " && ");
}
static void AST_AND_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_ADD */
static void AST_ADD_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_ADD_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " + ");
}
static void AST_ADD_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_SUB */
static void AST_SUB_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_SUB_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " - ");
}
static void AST_SUB_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_MUL */
static void AST_MUL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_MUL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " * ");
}
static void AST_MUL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_DIV */
static void AST_DIV_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_DIV_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " / ");
}
static void AST_DIV_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_MOD */
static void AST_MOD_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_MOD_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " %% ");
}
static void AST_MOD_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_ASSIGN */
static void AST_ASSIGN_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
}
static void AST_ASSIGN_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " = ");
}
static void AST_ASSIGN_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_CALL_EXPR */
static void AST_CALL_EXPR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_CALL_EXPR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, "(");
}
static void AST_CALL_EXPR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")");
}

/* AST_SUBSCRIPT_EXPR */
static void AST_SUBSCRIPT_EXPR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_SUBSCRIPT_EXPR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, "[");
}
static void AST_SUBSCRIPT_EXPR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "]");
}

/* AST_FN_DEF */
static void AST_FN_DEF_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "int ");
}
static void AST_FN_DEF_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  /*
	fprintf(fp, "(void)\n");
  */
}
static void AST_FN_DEF_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_FN_BODY */
static void AST_FN_BODY_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "(");
  if (node->lnode == NULL) {
    fprintf(fp, "void");
  }
}
static void AST_FN_BODY_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")\n");
}
static void AST_FN_BODY_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_ENUM_DEF */
static void AST_ENUM_DEF_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "enum ");
}
static void AST_ENUM_DEF_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, " {\n");
  cxt->is_inside_enum_def = 1;
  cxt->depth++;
}
static void AST_ENUM_DEF_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, "};\n");
  cxt->is_inside_enum_def = 0;
  cxt->depth--;
}

/* AST_ENUMERATOR */
static void AST_ENUMERATOR_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_ENUMERATOR_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL) {
    fprintf(fp, " = ");
  }
}
static void AST_ENUMERATOR_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_LIST */
static void AST_LIST_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (cxt->is_inside_enum_def) {
    indent(fp, cxt);
  }
}
static void AST_LIST_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (cxt->is_inside_enum_def) {
    if (node->rnode != NULL) {
      fprintf(fp, ",");
    }
    fprintf(fp, "\n");
  }
  else if (cxt->is_inside_initializer) {
    if (node->rnode != NULL) {
      fprintf(fp, ", ");
    }
  }
}
static void AST_LIST_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_LITERAL */
static void AST_LITERAL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  const char *literal = symbol_name(node->value.symbol);
  if (isalpha(literal[0]) && literal[1] == '\0') {
    fprintf(fp, "'%s'", literal);
  } else {
    fprintf(fp, "%s", literal);
  }
}
static void AST_LITERAL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_LITERAL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_STRING_LITERAL */
static void AST_STRING_LITERAL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, "\"%s\"", symbol_name(node->value.symbol));
}
static void AST_STRING_LITERAL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_STRING_LITERAL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_EMPTY_STMT */
static void AST_EMPTY_STMT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
}
static void AST_EMPTY_STMT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_EMPTY_STMT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}

/* AST_EXPR_STMT */
static void AST_EXPR_STMT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
}
static void AST_EXPR_STMT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_EXPR_STMT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}

/* AST_COMPOUND */
static void AST_COMPOUND_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "{\n");
  cxt->depth++;
}
static void AST_COMPOUND_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_COMPOUND_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(fp, cxt);
	fprintf(fp, "}\n");
}

/* AST_BREAK */
static void AST_BREAK_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "break");
}
static void AST_BREAK_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_BREAK_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}

/* AST_CONTINUE */
static void AST_CONTINUE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "continue");
}
static void AST_CONTINUE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_CONTINUE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}

/* AST_IF */
static void AST_IF_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "if (");
}
static void AST_IF_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
  fprintf(fp, ") ");
}
static void AST_IF_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_THEN */
static void AST_THEN_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_THEN_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL) {
    indent(fp, cxt);
    fprintf(fp, "else\n");
  }
}
static void AST_THEN_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_SWITCH */
static void AST_SWITCH_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "switch (");
}
static void AST_SWITCH_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  fprintf(fp, ")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(fp, node, cxt);
  }
}
static void AST_SWITCH_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(fp, node, cxt);
  }
}

/* AST_CASE */
static void AST_CASE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(fp, cxt);
  fprintf(fp, "case ");
  cxt->depth++;
}
static void AST_CASE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ":\n");
}
static void AST_CASE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_DEFAULT */
static void AST_DEFAULT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(fp, cxt);
  fprintf(fp, "default:");
  cxt->depth++;
}
static void AST_DEFAULT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_DEFAULT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "break;\n");
}

/* AST_FOR_INIT */
static void AST_FOR_INIT_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "for (");
}
static void AST_FOR_INIT_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  /*
	fprintf(fp, "; ");
  */
}
static void AST_FOR_INIT_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_FOR_COND */
static void AST_FOR_COND_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_FOR_COND_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  /*
	fprintf(fp, "; ");
  */
}
static void AST_FOR_COND_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_FOR_BODY */
static void AST_FOR_BODY_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_FOR_BODY_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(fp, node, cxt);
  }
}
static void AST_FOR_BODY_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(fp, node, cxt);
  }
}

/* AST_WHILE */
static void AST_WHILE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "while (");
}
static void AST_WHILE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(fp, node, cxt);
  }
}
static void AST_WHILE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(fp, node, cxt);
  }
}

/* AST_DO_WHILE */
static void AST_DO_WHILE_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "do\n");
  if (node->rnode != NULL && node->lnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(fp, node, cxt);
  }
}
static void AST_DO_WHILE_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->lnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(fp, node, cxt);
  }
  indent(fp, cxt);
	fprintf(fp, "while (");
}
static void AST_DO_WHILE_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ");\n");
}

/* AST_RETURN */
static void AST_RETURN_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "return ");
}
static void AST_RETURN_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_RETURN_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}

/* AST_GOTO */
static void AST_GOTO_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
	fprintf(fp, "goto ");
}
static void AST_GOTO_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ";\n");
}
static void AST_GOTO_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_LABEL */
static void AST_LABEL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(fp, cxt);
  cxt->depth++;
}
static void AST_LABEL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
	fprintf(fp, ":\n");
}
static void AST_LABEL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_SYMBOL */
static void AST_SYMBOL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
	if (strcmp(symbol_name(node->value.symbol), "print") == 0) {
		fprintf(fp, "printf");
	} else {
    fprintf(fp, "%s", symbol_name(node->value.symbol));
	}
}
static void AST_SYMBOL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_SYMBOL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
}

/* AST_VAR_DECL */
static void AST_VAR_DECL_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  node_t *idnt = node->lnode;
  const struct type_info type = symbol_type(idnt->value.symbol);

  indent(fp, cxt);
  if (type.kind == TYPE_BOOL) {
    fprintf(fp, "char ");
  } else if (type.kind == TYPE_STRING) {
    fprintf(fp, "char *");
  } else {
    fprintf(fp, "%s ", type_to_string(type.kind));
  }
}
static void AST_VAR_DECL_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
  node_t *idnt = node->lnode;
  const struct type_info type = symbol_type(idnt->value.symbol);
  if (type.is_array) {
    fprintf(fp, "[%lu]", type.array_size);
  }

	fprintf(fp, " = ");

  if (type.is_array) {
    fprintf(fp, "{");
    cxt->is_inside_initializer = 1;
  }
}
static void AST_VAR_DECL_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  node_t *idnt = node->lnode;
  const struct type_info type = symbol_type(idnt->value.symbol);
  if (type.is_array) {
    fprintf(fp, "}");
    cxt->is_inside_initializer = 0;
  }
	fprintf(fp, ";\n");
}

/* AST_VARDUMP */
static void AST_VARDUMP_pre_code(FILE *fp, const node_t *node, context_t *cxt)
{
  indent(fp, cxt);
  fprintf(fp, "printf(\"#  ");
}
static void AST_VARDUMP_in_code(FILE *fp, const node_t *node, context_t *cxt)
{
}
static void AST_VARDUMP_post_code(FILE *fp, const node_t *node, context_t *cxt)
{
  node_t *idnt = node->lnode;
  const struct type_info type = symbol_type(idnt->value.symbol);
  const char *name = symbol_name(idnt->value.symbol);
  const char *spec = "";
  switch (type.kind) {
  case TYPE_CHAR:
    fprintf(fp, " => '%%c' (char)\\n\", %s);\n", name);
    return;
    break;
  case TYPE_BOOL:
    fprintf(fp, " => %%s (bool)\\n\", %s==0?\"false\":\"true\");\n", name);
    return;
    break;
  case TYPE_SHORT:
  case TYPE_INT: spec = "%d"; break;
  case TYPE_LONG: spec = "%ld"; break;
  case TYPE_FLOAT:
  case TYPE_DOUBLE: spec = "%g"; break;
  case TYPE_STRING:
    fprintf(fp, " => \\\"%%s\\\" (string)\\n\", %s);\n", name);
    return;
    break;
  default: break;
  }
  fprintf(fp, " => %s (%s)\\n\", %s);\n", spec, type_to_string(type.kind), name);
}

typedef void (*WriteCode)(FILE *fp, const node_t *node, context_t *cxt);
typedef struct ccode {
	int ast_node_op;
	WriteCode write_pre_code;
	WriteCode write_in_code;
	WriteCode write_post_code;
} ccode_t;
static const ccode_t ccodes[] = {
#define T(tag, str) {tag, tag##_pre_code, tag##_in_code, tag##_post_code},
  AST_KIND_LIST(T)
#undef T
  {0, NULL, NULL, NULL}
};
static const int N_CCODES = sizeof(ccodes)/sizeof(ccodes[0]);

static void print_code_recursive(FILE *fp, const node_t *node, context_t *cxt)
{
	const ccode_t *ccode = NULL;
	int i;

	if (node == NULL) {
		return;
	}

	for (i = 0; i < N_CCODES; i++) {
		if (ccodes[i].ast_node_op == node->kind) {
			ccode = &ccodes[i];
			break;
		}
	}

	if (ccode != NULL) {
		ccode->write_pre_code(fp, node, cxt);
	}

	print_code_recursive(fp, node->lnode, cxt);

	if (ccode != NULL) {
		ccode->write_in_code(fp, node, cxt);
	}

	print_code_recursive(fp, node->rnode, cxt);

	if (ccode != NULL) {
		ccode->write_post_code(fp, node, cxt);
	}
}
