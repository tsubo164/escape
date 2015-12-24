/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "cgen.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct context context_t;
typedef struct ast_node node_t;

static void print_code_recursive(const node_t *node, context_t *cxt);

void print_c_code(const node_t *node, context_t *cxt)
{
  printf("#include <stdio.h>\n");
  printf("#define VARDUMP(var) printf(\"#  %%s => %%d (int)\\n\", #var, var)\n");
  print_code_recursive(node, cxt);
}

/*
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

static void indent(context_t *cxt)
{
	int i;
	for (i = 0; i < cxt->depth; i++) {
		printf("  ");
	}
}

/* AST_POST_INC */
static void AST_POST_INC_pre_code(const node_t *node, context_t *cxt)
{
	printf("((");
}
static void AST_POST_INC_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_POST_INC_post_code(const node_t *node, context_t *cxt)
{
	printf(")++)");
}

/* AST_POST_DEC */
static void AST_POST_DEC_pre_code(const node_t *node, context_t *cxt)
{
	printf("((");
}
static void AST_POST_DEC_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_POST_DEC_post_code(const node_t *node, context_t *cxt)
{
	printf(")--)");
}

/* AST_PRE_INC */
static void AST_PRE_INC_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_PRE_INC_in_code(const node_t *node, context_t *cxt)
{
	printf("++(");
}
static void AST_PRE_INC_post_code(const node_t *node, context_t *cxt)
{
	printf("))");
}

/* AST_PRE_DEC */
static void AST_PRE_DEC_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_PRE_DEC_in_code(const node_t *node, context_t *cxt)
{
	printf("--(");
}
static void AST_PRE_DEC_post_code(const node_t *node, context_t *cxt)
{
	printf("))");
}

/* AST_LSHIFT */
static void AST_LSHIFT_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_LSHIFT_in_code(const node_t *node, context_t *cxt)
{
	printf(" << ");
}
static void AST_LSHIFT_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_RSHIFT */
static void AST_RSHIFT_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_RSHIFT_in_code(const node_t *node, context_t *cxt)
{
	printf(" >> ");
}
static void AST_RSHIFT_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_LT */
static void AST_LT_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_LT_in_code(const node_t *node, context_t *cxt)
{
	printf(" < ");
}
static void AST_LT_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_GT */
static void AST_GT_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_GT_in_code(const node_t *node, context_t *cxt)
{
	printf(" > ");
}
static void AST_GT_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_LE */
static void AST_LE_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_LE_in_code(const node_t *node, context_t *cxt)
{
	printf(" <= ");
}
static void AST_LE_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_GE */
static void AST_GE_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_GE_in_code(const node_t *node, context_t *cxt)
{
	printf(" >= ");
}
static void AST_GE_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_EQ */
static void AST_EQ_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_EQ_in_code(const node_t *node, context_t *cxt)
{
	printf(" == ");
}
static void AST_EQ_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_NE */
static void AST_NE_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_NE_in_code(const node_t *node, context_t *cxt)
{
	printf(" != ");
}
static void AST_NE_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_BITWISE_AND */
static void AST_BITWISE_AND_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_BITWISE_AND_in_code(const node_t *node, context_t *cxt)
{
	printf(" & ");
}
static void AST_BITWISE_AND_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_BITWISE_XOR */
static void AST_BITWISE_XOR_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_BITWISE_XOR_in_code(const node_t *node, context_t *cxt)
{
	printf(" ^ ");
}
static void AST_BITWISE_XOR_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_BITWISE_OR */
static void AST_BITWISE_OR_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_BITWISE_OR_in_code(const node_t *node, context_t *cxt)
{
	printf(" | ");
}
static void AST_BITWISE_OR_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_OR */
static void AST_OR_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_OR_in_code(const node_t *node, context_t *cxt)
{
	printf(" || ");
}
static void AST_OR_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_AND */
static void AST_AND_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_AND_in_code(const node_t *node, context_t *cxt)
{
	printf(" && ");
}
static void AST_AND_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_ADD */
static void AST_ADD_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_ADD_in_code(const node_t *node, context_t *cxt)
{
	printf(" + ");
}
static void AST_ADD_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_SUB */
static void AST_SUB_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_SUB_in_code(const node_t *node, context_t *cxt)
{
	printf(" - ");
}
static void AST_SUB_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_MUL */
static void AST_MUL_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_MUL_in_code(const node_t *node, context_t *cxt)
{
	printf(" * ");
}
static void AST_MUL_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_DIV */
static void AST_DIV_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_DIV_in_code(const node_t *node, context_t *cxt)
{
	printf(" / ");
}
static void AST_DIV_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_MOD */
static void AST_MOD_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_MOD_in_code(const node_t *node, context_t *cxt)
{
	printf(" %% ");
}
static void AST_MOD_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_ASSIGN */
static void AST_ASSIGN_pre_code(const node_t *node, context_t *cxt)
{
	printf("(");
}
static void AST_ASSIGN_in_code(const node_t *node, context_t *cxt)
{
	printf(" = ");
}
static void AST_ASSIGN_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}

/* AST_CALL_EXPR */
#if 0
static void AST_CALL_EXPR_pre_code(const node_t *node, context_t *cxt)
{
}
static void AST_CALL_EXPR_in_code(const node_t *node, context_t *cxt)
{
  printf("(");
}
static void AST_CALL_EXPR_post_code(const node_t *node, context_t *cxt)
{
	printf(")");
}
#endif

/* AST_FN_DEF */
static void AST_FN_DEF_pre_code(const node_t *node, context_t *cxt)
{
	const char *function_name = node->value.word;
	printf("int %s(void", function_name);
}
static void AST_FN_DEF_in_code(const node_t *node, context_t *cxt)
{
	printf(")\n");
}
static void AST_FN_DEF_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_LIST */
static void AST_LIST_pre_code(const node_t *node, context_t *cxt)
{
}
static void AST_LIST_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_LIST_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_LITERAL */
static void AST_LITERAL_pre_code(const node_t *node, context_t *cxt)
{
  printf("%s", node->value.word);
}
static void AST_LITERAL_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_LITERAL_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_EMPTY_STMT */
static void AST_EMPTY_STMT_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
}
static void AST_EMPTY_STMT_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_EMPTY_STMT_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_EXPR_STMT */
static void AST_EXPR_STMT_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
}
static void AST_EXPR_STMT_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_EXPR_STMT_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_COMPOUND */
static void AST_COMPOUND_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("{\n");
  cxt->depth++;
}
static void AST_COMPOUND_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_COMPOUND_post_code(const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(cxt);
	printf("}\n");
}

/* AST_BREAK */
static void AST_BREAK_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("break");
}
static void AST_BREAK_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_BREAK_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_CONTINUE */
static void AST_CONTINUE_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("continue");
}
static void AST_CONTINUE_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_CONTINUE_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_IF */
static void AST_IF_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("if (");
}
static void AST_IF_in_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
  printf(") ");
}
static void AST_IF_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_THEN */
static void AST_THEN_pre_code(const node_t *node, context_t *cxt)
{
}
static void AST_THEN_in_code(const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL) {
    indent(cxt);
    printf("else\n");
  }
}
static void AST_THEN_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_SWITCH */
static void AST_SWITCH_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("switch (");
}
static void AST_SWITCH_in_code(const node_t *node, context_t *cxt)
{
  printf(")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(node, cxt);
  }
}
static void AST_SWITCH_post_code(const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(node, cxt);
  }
}

/* AST_CASE */
static void AST_CASE_pre_code(const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(cxt);
  if (node->lnode != NULL) {
    printf("case ");
  } else {
    printf("default");
  }
  cxt->depth++;
}
static void AST_CASE_in_code(const node_t *node, context_t *cxt)
{
	printf(":\n");
}
static void AST_CASE_post_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("break;\n");
}

/* AST_FOR_INIT */
static void AST_FOR_INIT_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("for (");
}
static void AST_FOR_INIT_in_code(const node_t *node, context_t *cxt)
{
	printf("; ");
}
static void AST_FOR_INIT_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_FOR_COND */
static void AST_FOR_COND_pre_code(const node_t *node, context_t *cxt)
{
}
static void AST_FOR_COND_in_code(const node_t *node, context_t *cxt)
{
	printf("; ");
}
static void AST_FOR_COND_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_FOR_BODY */
static void AST_FOR_BODY_pre_code(const node_t *node, context_t *cxt)
{
}
static void AST_FOR_BODY_in_code(const node_t *node, context_t *cxt)
{
	printf(")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(node, cxt);
  }
}
static void AST_FOR_BODY_post_code(const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(node, cxt);
  }
}

/* AST_WHILE */
static void AST_WHILE_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("while (");
}
static void AST_WHILE_in_code(const node_t *node, context_t *cxt)
{
	printf(")\n");
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(node, cxt);
  }
}
static void AST_WHILE_post_code(const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->rnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(node, cxt);
  }
}

/* AST_DO_WHILE */
static void AST_DO_WHILE_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("do\n");
  if (node->rnode != NULL && node->lnode->kind != AST_COMPOUND) {
    AST_COMPOUND_pre_code(node, cxt);
  }
}
static void AST_DO_WHILE_in_code(const node_t *node, context_t *cxt)
{
  if (node->rnode != NULL && node->lnode->kind != AST_COMPOUND) {
    AST_COMPOUND_post_code(node, cxt);
  }
  indent(cxt);
	printf("while (");
}
static void AST_DO_WHILE_post_code(const node_t *node, context_t *cxt)
{
	printf(");\n");
}

/* AST_RETURN */
static void AST_RETURN_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("return ");
}
static void AST_RETURN_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_RETURN_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_GOTO */
static void AST_GOTO_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("goto ");
}
static void AST_GOTO_in_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}
static void AST_GOTO_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_LABEL */
static void AST_LABEL_pre_code(const node_t *node, context_t *cxt)
{
  cxt->depth--;
  indent(cxt);
  cxt->depth++;
}
static void AST_LABEL_in_code(const node_t *node, context_t *cxt)
{
	printf(":\n");
}
static void AST_LABEL_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_SYMBOL */
static void AST_SYMBOL_pre_code(const node_t *node, context_t *cxt)
{
	if (strcmp(node->value.word, "print") == 0) {
		printf("printf");
	} else {
		printf("%s", node->value.word);
	}
}
static void AST_SYMBOL_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_SYMBOL_post_code(const node_t *node, context_t *cxt)
{
}

/* AST_VAR_DECL */
static void AST_VAR_DECL_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);
	printf("int ");
#if 0
  const char *ctype_name = NULL;

  switch (node->value.symbol->data_type) {
  case TYPE_BOOL:
  case TYPE_CHAR:
    ctype_name = "char";
    break;
  case TYPE_SHORT:
    ctype_name = "short";
    break;
  case TYPE_INT:
    ctype_name = "int";
    break;
  case TYPE_LONG:
    ctype_name = "long";
    break;
  case TYPE_FLOAT:
    ctype_name = "float";
    break;
  case TYPE_DOUBLE:
    ctype_name = "double";
    break;
  default:
    ctype_name = "error!";
    break;
  }

  indent(cxt);
	printf("%s ", ctype_name);
	printf("%s",  node->value.symbol->name);
  if (node->rnode == NULL) {
    printf(" = (%s) 0", ctype_name);
  } else {
    printf(" = (%s) ", ctype_name);
  }
#endif
}
static void AST_VAR_DECL_in_code(const node_t *node, context_t *cxt)
{
	printf(" = ");
}
static void AST_VAR_DECL_post_code(const node_t *node, context_t *cxt)
{
	printf(";\n");
}

/* AST_VARDUMP */
static void AST_VARDUMP_pre_code(const node_t *node, context_t *cxt)
{
  indent(cxt);

  printf("VARDUMP(");
#if 0
  printf("#define VARDUMP(var) printf(\"#  \"#var\" => %%d (int)\\n\", var)\n");
  switch (node->value.symbol->data_type) {
  case TYPE_BOOL:
    printf("printf(\"#  %s => %%d (bool)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_CHAR:
    printf("printf(\"#  %s => '%%c' (char)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_SHORT:
    printf("printf(\"#  %s => %%d (short)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_INT:
    printf("printf(\"#  %s => %%d (int)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_LONG:
    printf("printf(\"#  %s => %%ld (long)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_FLOAT:
    printf("printf(\"#  %s => %%g (float)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  case TYPE_DOUBLE:
    printf("printf(\"#  %s => %%g (double)\\n\", %s)",
        node->value.symbol->name,
        node->value.symbol->name);
    break;
  default:
    break;
  }
#endif
}
static void AST_VARDUMP_in_code(const node_t *node, context_t *cxt)
{
}
static void AST_VARDUMP_post_code(const node_t *node, context_t *cxt)
{
  printf(");\n");
}

typedef void (*WriteCode)(const node_t *node, context_t *cxt);
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

static void print_code_recursive(const node_t *node, context_t *cxt)
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
		ccode->write_pre_code(node, cxt);
	}

	print_code_recursive(node->lnode, cxt);

	if (ccode != NULL) {
		ccode->write_in_code(node, cxt);
	}

	print_code_recursive(node->rnode, cxt);

	if (ccode != NULL) {
		ccode->write_post_code(node, cxt);
	}
}
