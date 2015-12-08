/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef LEXER_H
#define LEXER_H

#include "stream.h"
#include <stdio.h>

#define TOKEN_LIST(T) \
  T(TK_DEC, "decrement") \
  T(TK_INC, "increment") \
  T(TK_ADD_ASSIGN, "add assign") \
  T(TK_SUB_ASSIGN, "sub assign") \
  T(TK_MUL_ASSIGN, "mul assign") \
  T(TK_DIV_ASSIGN, "div assign") \
  T(TK_LSHIFT, "left shift") \
  T(TK_RSHIFT, "right shift") \
  T(TK_EQ, "equal") \
  T(TK_NE, "not equal") \
  T(TK_LE, "less than equal") \
  T(TK_GE, "greater than equal") \
  T(TK_AND, "and") \
  T(TK_OR, "or") \
  T(TK_INT_LITERAL, "int literal") \
  T(TK_FLOAT_LITERAL, "float literal") \
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

#define TOKBUF_SIZE 2

struct lexer {
  int line;
  int column;
  struct stream strm;
  struct token tokbuf[TOKBUF_SIZE];
  int tokcurr;
  int is_head;
};

#define LEXER_INIT {1,0,STREAM_INIT,{TOKEN_INIT},0,1}

extern int lex_input_string(struct lexer *l, const char *string);
extern int lex_input_file(struct lexer *l, const char *filename);
extern void lex_finish(struct lexer *l);

extern struct token *lex_get_token(struct lexer *l);
extern struct token *lex_unget_token(struct lexer *l);

extern int lex_get_line_num(const struct lexer *l);
extern int lex_get_column_num(const struct lexer *l);

#endif /* XXX_H */
