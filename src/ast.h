/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef AST_H
#define AST_H

#define AST_KIND_LIST(T) \
  T(AST_LITERAL, "literal") \
  T(AST_LIST, "list") \
  T(AST_BREAK, "break") \
  T(AST_CONTINUE, "continue") \
  T(AST_RETURN, "return") \
  T(AST_VAR_DECL, "var_decl")

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
    /* int literal */
    long ivalue;
    /* list */
    struct {
      struct ast_node *next;
    };
    /* var decl */
    struct {
      struct ast_node *init;
    };
  };
};

#define NODE_INIT {0,NULL,NULL}

extern void ast_print_tree(const struct ast_node *node);

#if 0
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
  NODE_GOTO_STMT,
  NODE_BREAK_STMT,
  NODE_LABELED_STMT,
  NODE_CONTINUE_STMT,
  NODE_RETURN_STMT,
  NODE_NULL_STMT,
  /* literals */
  NODE_INT_LITERAL,
  NODE_FLOAT_LITERAL,
  NODE_STRING_LITERAL,
  NODE_SYMBOL,
  NODE_VAR_DECL,
  NODE_VARDUMP,
  NODE_END
};

struct AstNode {
  int op;
  union {
    long Integer;
    double Float;
    struct Symbol *symbol;
  } value;

  /* TODO TEST */
  int data_type;

  struct AstNode *left;
  struct AstNode *right;
};

extern struct AstNode *AstNode_New(int ast_node_op);
extern void AstNode_Free(struct AstNode *node);

extern int AstNode_Op(const struct AstNode *node);
extern struct AstNode *AstNode_Left(const struct AstNode *node);
extern struct AstNode *AstNode_Right(const struct AstNode *node);
extern struct Symbol *AstNode_Symbol(const struct AstNode *node);
extern long AstNode_IntegerValue(const struct AstNode *node);

extern void AstNode_Print(const struct AstNode *node);
#endif

#endif /* XXX_H */
