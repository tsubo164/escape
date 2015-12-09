/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

#define TOKEN_LIST(T) \
  T(TK_INC, "++") \
  T(TK_DEC, "--") \
  T(TK_ADD_ASSIGN, "+=") \
  T(TK_SUB_ASSIGN, "-=") \
  T(TK_MUL_ASSIGN, "*=") \
  T(TK_DIV_ASSIGN, "/=") \
  T(TK_LSHIFT, "<<") \
  T(TK_RSHIFT, ">>") \
  T(TK_EQ, "==") \
  T(TK_NE, "!!") \
  T(TK_LE, "<=") \
  T(TK_GE, ">=") \
  T(TK_AND, "&&") \
  T(TK_OR, "||") \
  T(TK_INT_LITERAL, "int_literal") \
  T(TK_FLOAT_LITERAL, "float_literal") \
  T(TK_IDENT, "identifier")

#define KEYWORD_LIST(T) \
  T(TK_BOOL, "bool") \
  T(TK_BREAK, "break") \
  T(TK_CASE, "case") \
  T(TK_CONTINUE, "continue") \
  T(TK_DEFAULT, "default") \
  T(TK_DO, "do") \
  T(TK_ELSE, "else") \
  T(TK_ENUM, "enum") \
  T(TK_FALSE, "false") \
  T(TK_FN, "fn") \
  T(TK_FOR, "for") \
  T(TK_goto, "goto") \
  T(TK_IF, "if") \
  T(TK_INT, "int") \
  T(TK_NULL, "null") \
  T(TK_RETURN, "return") \
  T(TK_STRUCT, "struct") \
  T(TK_SWITCH, "switch") \
  T(TK_TRUE, "true") \
  T(TK_VAR, "var") \
  T(TK_WHILE, "while")

enum token_kind {
  TK_BEGIN = 256,
#define T(tag,str) tag,
  TOKEN_LIST(T)
  KEYWORD_BEGIN,
  KEYWORD_LIST(T)
#undef T
  TK_END
};

#define TOKEN_MAX_WORD_SIZE 128

struct token {
  int kind;
  union {
    long Integer;
    double Float;
    char word[TOKEN_MAX_WORD_SIZE];
  } value;
};

#define TOKEN_INIT {0,{0}}

extern int kind_of(const struct token *tok);
extern int int_value_of(const struct token *tok);
extern float float_value_of(const struct token *tok);
extern const char *word_value_of(const struct token *tok);

extern const char *token_to_string(const struct token *tok);

#endif /* XXX_H */
