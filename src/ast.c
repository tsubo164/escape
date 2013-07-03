/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "ast.h"
#include "datatype.h"
#include "memory.h"
#include "symbol.h"

#include <stdio.h>
#include <assert.h>

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

double AstNode_Number(const struct AstNode *node)
{
  return node->value.number;
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
  case NODE_NUMBER_LITERAL:
    printf("%g", node->value.number);
    break;
  case NODE_ASSIGN:
    printf("assignment");
    break;
  case NODE_BLOCK:
    printf("block");
    break;
  case NODE_EXPR:
    printf("expression");
    break;
  case NODE_FUNC_DEF:
    printf("function definition: [%s]", node->value.symbol->name);
    break;
  case NODE_FUNC_CALL:
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

