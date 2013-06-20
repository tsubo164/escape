/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

enum TokenTag {
  /* tokens */
  TK_BEGIN_TOKENS = 256,
  TK_COMMENT,
  TK_IDENTIFIER,
  TK_NUMBER,
  TK_STRING,
  /* keywords */
  TK_BEGIN_KEYWORDS,
  TK_KW_FUNCTION,
  TK_KW_INT,
  TK_KW_STRING,
  TK_KW_RETURN,
  TK_KW_VAR,
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

