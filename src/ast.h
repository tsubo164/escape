/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef AST_H
#define AST_H

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
  T(AST_LIST, "list") \
  T(AST_LITERAL, "literal") \
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
  } value;
};

#define NODE_INIT {0,NULL,NULL}

extern void ast_print_tree(const struct ast_node *node);

extern void print_c_code(const struct ast_node *node);

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
