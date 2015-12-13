/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "ast.h"
#include <stdio.h>
#include <assert.h>

typedef struct ast_node node_t;
static void print_node_recursive(const node_t *node, int depth);

void ast_print_tree(const struct ast_node *node)
{
  print_node_recursive(node, 0);
}

static const struct {
  int kind;
  const char *string;
} ast_table[] = {
#define T(kind,str) {kind,str},
  AST_KIND_LIST(T)
#undef T
  {0,""} /* for no-comma entry */
};

static void print_node_recursive(const node_t *node, int depth)
{
  int i;
  int next_depth = depth;

  if (node == NULL) {
    return;
  }

  for (i = 0; i < depth; i++) {
    printf("  ");
  }

  if (node->kind != AST_LIST) {
    printf("%s\n", ast_table[node->kind].string);
    next_depth++;
  }

  print_node_recursive(node->lnode, depth);
  print_node_recursive(node->rnode, depth);
}

#if 0
static void free_node_recursive(struct AstNode *node);
static void print_node_recursive(const struct AstNode *node, int depth);

struct AstNode *AstNode_New(int ast_node_op)
{
  struct AstNode *node = NULL;

  assert(ast_node_op < NODE_END);

  node = MEMORY_ALLOC(struct AstNode);
  if (node == NULL) {
    return NULL;
  }

  node->op = ast_node_op;
  node->value.symbol = NULL;
  node->data_type = TYPE_NONE;

  node->left = NULL;
  node->right = NULL;

  return node;
}

void AstNode_Free(struct AstNode *node)
{
  free_node_recursive(node);
}

void AstNode_Print(const struct AstNode *node)
{
  print_node_recursive(node, 0);
}

int AstNode_Op(const struct AstNode *node)
{
  return node->op;
}

struct AstNode *AstNode_Left(const struct AstNode *node)
{
  return node->left;
}

struct AstNode *AstNode_Right(const struct AstNode *node)
{
  return node->right;
}

struct Symbol *AstNode_Symbol(const struct AstNode *node)
{
  return node->value.symbol;
}

long AstNode_IntegerValue(const struct AstNode *node)
{
  return node->value.Integer;
}

static void free_node_recursive(struct AstNode *node)
{
  if (node == NULL) {
    return;
  }

  free_node_recursive(node->left);
  free_node_recursive(node->right);

  MEMORY_FREE(node);
}

static void print_node_recursive(const struct AstNode *node, int depth)
{
  int i;

  if (node == NULL) {
    return;
  }

  for (i = 0; i < depth; i++) {
    printf("  ");
  }

  switch (node->op) {
  case NODE_NULL:
    printf("null");
    break;
  case NODE_ADD:
    printf("+");
    break;
  case NODE_SUB:
    printf("-");
    break;
  case NODE_MUL:
    printf("*");
    break;
  case NODE_INT_LITERAL:
    printf("%ld", node->value.Integer);
    break;
  case NODE_FLOAT_LITERAL:
    printf("%g", node->value.Float);
    break;
  case NODE_ASSIGN:
    printf("assignment");
    break;
  case NODE_BLOCK:
    printf("block");
    break;
  case NODE_FUNC_DEF:
    printf("function definition: [%s]", node->value.symbol->name);
    break;
  case NODE_CALL_EXPR:
    printf("function call: [%s]", node->value.symbol->name);
    break;
  case NODE_LIST:
    printf("list node");
    break;
  case NODE_RETURN_STMT:
    printf("return statement");
    break;
  case NODE_VAR_DECL:
    printf("variable declaration");
    break;
  case NODE_END:
    printf("END");
    break;
  default:
    break;
  }

  printf("\n");

  print_node_recursive(node->left, depth + 1);
  print_node_recursive(node->right, depth + 1);
}
#endif
