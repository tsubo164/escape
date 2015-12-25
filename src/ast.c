/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "ast.h"
#include "memory.h"
#include <stdio.h>
#include <assert.h>

typedef struct ast_node node_t;
static void print_node_recursive(const node_t *node, int depth);
static void free_node_recursive(node_t *node);

struct ast_node *new_node(int kind, struct ast_node *left, struct ast_node *right)
{
  node_t *n = MEMORY_ALLOC(node_t);
  n->kind = kind;
  n->lnode = left;
  n->rnode = right;
  return n;
}

void ast_print_tree(const struct ast_node *node)
{
  print_node_recursive(node, 0);
}

void ast_free_node(struct ast_node *node)
{
  free_node_recursive(node);
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
    if (i == 0) {
      printf("  |");
    } else {
      printf("--|");
    }
  }

  if (node->kind != AST_LIST) {
    switch (node->kind) {
    case AST_LITERAL:
      printf("%s [%s]\n", ast_table[node->kind].string, node->value.word);
      break;
    case AST_SYMBOL:
      printf("%s [%s]\n", ast_table[node->kind].string, symbol_name(node->value.symbol));
      break;

    default:
      printf("%s\n", ast_table[node->kind].string);
      break;
    }
    next_depth++;
  } else {
    printf("\n");
  }

  print_node_recursive(node->lnode, next_depth);
  print_node_recursive(node->rnode, next_depth);
}

void free_node_recursive(node_t *node)
{
  if (node == NULL) {
    return;
  }
  free_node_recursive(node->lnode);
  free_node_recursive(node->rnode);
  MEMORY_FREE(node);
}
