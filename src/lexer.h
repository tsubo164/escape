/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef LEXER_H
#define LEXER_H

#include "stream.h"
#include "token.h"
#include <stdio.h>

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

extern const struct token *lex_get_token(struct lexer *l);
extern const struct token *lex_unget_token(struct lexer *l);
extern const struct token *lex_current_token(const struct lexer *l);

extern int lex_get_line_num(const struct lexer *l);
extern int lex_get_column_num(const struct lexer *l);

#endif /* XXX_H */
