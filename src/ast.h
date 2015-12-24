/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef AST_H
#define AST_H

#include "symbol.h"

#define AST_KIND_LIST(T) \
  T(AST_ASSIGN, "=") \
  T(AST_OR, "||") \
  T(AST_AND, "&&") \
  T(AST_BITWISE_OR, "|") \
  T(AST_BITWISE_XOR, "^") \
  T(AST_BITWISE_AND, "&") \
  T(AST_EQ, "==") \
  T(AST_NE, "!=") \
  T(AST_LT, "<") \
  T(AST_GT, ">") \
  T(AST_LE, "<=") \
  T(AST_GE, ">=") \
  T(AST_LSHIFT, "<<") \
  T(AST_RSHIFT, ">>") \
  T(AST_ADD, "+") \
  T(AST_SUB, "-") \
  T(AST_MUL, "*") \
  T(AST_DIV, "/") \
  T(AST_MOD, "%") \
  T(AST_PRE_INC, "++()") \
  T(AST_PRE_DEC, "--()") \
  T(AST_POST_INC, "()++") \
  T(AST_POST_DEC, "()--") \
  T(AST_LIST, "list") \
  T(AST_LITERAL, "literal") \
  T(AST_EMPTY_STMT, "empty_statement") \
  T(AST_EXPR_STMT, "expression_statement") \
  T(AST_IF, "if") \
  T(AST_THEN, "then") \
  T(AST_SWITCH, "switch") \
  T(AST_FOR_INIT, "for") \
  T(AST_FOR_COND, "cond") \
  T(AST_FOR_BODY, "body") \
  T(AST_WHILE, "while") \
  T(AST_DO_WHILE, "do_while") \
  T(AST_COMPOUND, "compound") \
  T(AST_LABEL, "label") \
  T(AST_CASE, "case") \
  T(AST_SYMBOL, "symbol") \
  T(AST_BREAK, "break") \
  T(AST_CONTINUE, "continue") \
  T(AST_GOTO, "goto") \
  T(AST_RETURN, "return") \
  T(AST_FN_DEF, "fn_def") \
  T(AST_VAR_DECL, "var_decl") \
  T(AST_VARDUMP, "vardump")

enum ast_kind {
#define T(tag,str) tag,
  AST_KIND_LIST(T)
#undef T
  AST_NUL
};

struct ast_node {
  int kind;
  struct ast_node *lnode;
  struct ast_node *rnode;

  union {
    char word[128];
    struct symbol *symbol;
  } value;
};

#define NODE_INIT {0,NULL,NULL,{0}}

extern struct ast_node *new_node(int kind, struct ast_node *left, struct ast_node *right);
extern void ast_print_tree(const struct ast_node *node);
extern void ast_free_node(struct ast_node *node);

#endif /* XXX_H */
