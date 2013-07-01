/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define TOKEN_LIST(t_) \
  t_(TK_COMMENT,        "") \
  t_(TK_IDENTIFIER,     "") \
  t_(TK_NUMBER,         "") \
  t_(TK_STRING,         "") \
  /* relational operators */ \
  t_(TK_LE,             "<") \
  t_(TK_GE,             ">") \
  t_(TK_EQ,             "==") \
  t_(TK_NE,             "!=") \
  t_(TK_AND,            "&&") \
  t_(TK_OR,             "||") \
  /* keywords */ \
  t_(TK_BEGIN_KEYWORDS, "") \
  t_(TK_KW_FUNCTION,    "function") \
  t_(TK_KW_IF,          "if") \
  t_(TK_KW_INT,         "int") \
  t_(TK_KW_STRING,      "string") \
  t_(TK_KW_RETURN,      "return") \
  t_(TK_KW_VAR,         "var") \
  t_(TK_KW_VARDUMP,     "vardump")

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

