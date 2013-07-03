/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define TOKEN_LIST(tk_, kw_) \
  tk_(TK_EOS,            "") \
  tk_(TK_COMMENT,        "") \
  tk_(TK_IDENTIFIER,     "") \
  tk_(TK_NUMBER,         "") \
  tk_(TK_STRING_LITERAL, "") \
  /* operators */ \
  tk_(MUL,               "*") \
  tk_(DIV,               "/") \
  tk_(ADD,               "+") \
  tk_(SUB,               "-") \
  tk_(TK_LSHIFT,         "<<") \
  tk_(TK_RSHIFT,         ">>") \
  tk_(TK_LE,             "<=") \
  tk_(TK_GE,             ">=") \
  tk_(TK_EQ,             "==") \
  tk_(TK_NE,             "!=") \
  tk_(TK_AND,            "&&") \
  tk_(TK_OR,             "||") \
  /* keywords */ \
  kw_(TK_FUNCTION,       "function") \
  kw_(TK_IF,             "if") \
  kw_(TK_INT,            "int") \
  kw_(TK_STRING,         "string") \
  kw_(TK_RETURN,         "return") \
  kw_(TK_VAR,            "var") \
  kw_(TK_VARDUMP,        "vardump")

enum TokenTag {
  /* tokens */
  TK_BEGIN_TOKENS = 256,
  TK_COMMENT,
  TK_IDENTIFIER,
  TK_NUMBER,
  TK_STRING,
  /* relational operators */
  TK_LSHIFT,
  TK_RSHIFT,
  TK_LE,
  TK_GE,
  TK_EQ,
  TK_NE,
  TK_AND,
  TK_OR,
  /* keywords */
  TK_BEGIN_KEYWORDS,
  TK_KW_ELSE,
  TK_KW_FUNCTION,
  TK_KW_IF,
  TK_KW_INT,
  TK_KW_STRING,
  TK_KW_RETURN,
  TK_KW_VAR,
  TK_KW_VARDUMP,
  TK_END_TOKENS
};

enum { MAX_NAME_SIZE = 64 };

struct Token {
  int tag;
  union {
    double number;
    char name[MAX_NAME_SIZE];
    const char *string;
  } value;
  int data_type;
};

struct Lexer;

extern struct Lexer *Lexer_New(void);
extern void Lexer_Free(struct Lexer *lexer);

extern void Lexer_SetInputFile(struct Lexer *lexer, FILE *file);
extern void Lexer_SetInputString(struct Lexer *lexer, const char *str);
extern int Lexer_NextToken(struct Lexer *lexer, struct Token *token);
extern int Lexer_GetLineNumber(const struct Lexer *lexer);

#endif /* XXX_H */

