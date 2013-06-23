/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef AST_H
#define AST_H

struct Symbol;

enum AstNodeType {
	NODE_NULL = 0,
	NODE_ASSIGN,
	NODE_BLOCK,
	NODE_EXPR,
	NODE_FUNC_DEF,
	NODE_FUNC_CALL,
	NODE_LIST,
	NODE_RETURN_STMT,
	NODE_STRING_LITERAL,
	NODE_VAR_DECL,
	NODE_VARDUMP,
	NODE_END
};

struct AstNode {
	int type;
	union {
		double number;
		struct Symbol *symbol;
	} value;

	struct AstNode *left;
	struct AstNode *right;
};

extern struct AstNode *AstNode_Duplicate(const struct AstNode *src);
extern struct AstNode *AstNode_New(int ast_node_type);
extern void AstNode_Free(struct AstNode *node);

extern void AstNode_Print(const struct AstNode *node);

#endif /* XXX_H */

