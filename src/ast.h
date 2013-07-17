/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef AST_H
#define AST_H

struct Symbol;

enum AstNodeType {
  NODE_NULL = 0,
  /* operators */
  NODE_POST_INC,
  NODE_POST_DEC,
  NODE_INC,
  NODE_DEC,
  NODE_LSHIFT,
  NODE_RSHIFT,
  NODE_LT,
  NODE_GT,
  NODE_LE,
  NODE_GE,
  NODE_EQ,
  NODE_NE,
  NODE_BITWISE_AND,
  NODE_BITWISE_XOR,
  NODE_BITWISE_OR,
  NODE_OR,
  NODE_AND,
  NODE_ADD,
  NODE_SUB,
  NODE_MUL,
  NODE_DIV,
  NODE_MOD,
  NODE_ASSIGN,
  /* control structures */
  NODE_IF,
  NODE_COND,
  NODE_SWITCH,
  NODE_CASE_STMT,
  NODE_WHILE,
  NODE_DO_WHILE,
  NODE_FOR_INIT,
  NODE_FOR_COND,
  NODE_FOR_LOOP,
  NODE_LIST,
  NODE_BLOCK,
  NODE_CALL_EXPR,
  NODE_FUNC_DEF,
  NODE_EXPR_STMT,
  NODE_BREAK_STMT,
  NODE_CONTINUE_STMT,
  NODE_RETURN_STMT,
  NODE_NULL_STMT,
  /* literals */
  NODE_NUMBER_LITERAL,
  NODE_STRING_LITERAL,
  NODE_SYMBOL,
  NODE_VAR_DECL,
  NODE_VARDUMP,
  NODE_END
};

struct AstNode {
  int op;
  union {
    double number;
    struct Symbol *symbol;
  } value;

  struct AstNode *left;
  struct AstNode *right;
};

extern struct AstNode *AstNode_New(int ast_node_op);
extern void AstNode_Free(struct AstNode *node);

extern int AstNode_Op(const struct AstNode *node);
extern struct AstNode *AstNode_Left(const struct AstNode *node);
extern struct AstNode *AstNode_Right(const struct AstNode *node);
extern struct Symbol *AstNode_Symbol(const struct AstNode *node);
extern double AstNode_Number(const struct AstNode *node);

extern void AstNode_Print(const struct AstNode *node);

#endif /* XXX_H */

