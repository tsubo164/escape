/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "lexer.h"
#include "memory.h"
#include "token.h"
#include <string.h>
#include <ctype.h>

static void fwd_tokbuf(struct lexer *l)
{
  l->tokcurr = (l->tokcurr + 1) % TOKBUF_SIZE;
}

static void bwd_tokbuf(struct lexer *l)
{
  l->tokcurr = (l->tokcurr - 1 + TOKBUF_SIZE) % TOKBUF_SIZE;
}

static void save_tok(struct lexer *l, const struct token *tok)
{
  l->tokbuf[l->tokcurr] = *tok;
}

static const struct token *load_tok(const struct lexer *l)
{
  return &l->tokbuf[l->tokcurr];
}

static char get_ch(struct lexer *l)
{
  const char c = stream_getc(&l->strm);
  l->column++;
  return c;
}

static char unget_ch(struct lexer *l)
{
  const char c = stream_ungetc(&l->strm);
  l->column--;
  return c;
}

static void detect_newline(struct lexer *l)
{
  l->line++;
  l->column = 0;
}

static const struct keyword {
  int id;
  const char *word;
} keywords[] = {
#define T(id,str) {id, str},
  KEYWORD_LIST(T)
#undef T
  {0, ""}
};

static const size_t N_KEYWORDS = sizeof(keywords)/sizeof(keywords[0]);
static void keyword_or_identifier(struct token *tok)
{
  int i;

  for (i = 0; i < N_KEYWORDS; i++) {
    const struct keyword *key = &keywords[i];
    if (strcmp(tok->value.word, key->word) == 0) {
      tok->kind = key->id;
      return;
    }
  }
  tok->kind = TK_IDENTIFIER;
}

static int isidentifier(char c)
{
  return isalnum(c) || c == '_';
}

static char scan_word(struct lexer *l, struct token *tok)
{
  char *dst = tok->value.word;
  char c = '\0';

  while ((c = get_ch(l)) != '\0') {
    if (isidentifier(c)) {
      *dst++ = c;
    } else {
      c = unget_ch(l);
      break;
    }
  }
  *dst = '\0';

  keyword_or_identifier(tok);
  return c;
}

enum number_type {
  NUM_INT,
  NUM_DOUBLE,
  NUM_FLOAT
};

static char scan_number(struct lexer *l, struct token *tok)
{
  char *dst = tok->value.word;
  char c = '\0';
  char prev = c;
  int has_e = 0;
  int has_x = 0;
  int has_pm = 0;
  int has_dot = 0;

  c = get_ch(l);

  if (c != '.' && isdigit(c) != 0) {
    /* TODO assert */
  }
  *dst++ = prev = c;

  for (;;) {
    c = get_ch(l);

    if (isdigit(c)) {
      *dst++ = prev = c;
    }
    else if (toupper(c)=='X' && prev=='0' && has_x==0) {
      *dst++ = prev = c;
      has_x = 1;
    }
    else if (toupper(c)=='E' && prev!='.' && has_e==0) {
      *dst++ = prev = c;
      has_e = 1;
    }
    else if ((c=='+' || c=='-') && toupper(prev)=='E' && has_pm==0) {
      *dst++ = prev = c;
      has_pm = 1;
    }
    else if (c=='.' && has_dot==0) {
      *dst++ = prev = c;
      has_dot = 1;
    }
    else if (toupper(c)=='F') {
      *dst++ = prev = c;
      break;
    }
    else if (toupper(c)=='X' || toupper(c)=='U' || toupper(c)=='L') {
      *dst++ = prev = c;
      break;
    }
    else {
      unget_ch(l);
      break;
    }
  }

  *dst = '\0';
  tok->kind = TK_NUMBER;

  return prev;
}

#if 0
static char scan_number(struct lexer *l, struct token *tok)
{
  char buf[265] = {'\0'};
  char ch = '\0';
  char *end = NULL;
  int i = 0;
  int num_type = NUM_INT;

state_initial:
  ch = get_ch(l);

  switch (ch) {
  case '+': case '-':
    if (i > 0) {
      num_type = NUM_DOUBLE;
    }
    buf[i++] = ch;
    goto state_initial;

  case '.': case 'e': case 'E':
    num_type = NUM_DOUBLE;
    buf[i++] = ch;
    goto state_initial;

  case 'f': case 'F':
    num_type = NUM_FLOAT;
    buf[i++] = ch;
    buf[i] = '\0';
    break;

  case 'x': case 'X':
  case 'u': case 'U':
  case 'l': case 'L':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    buf[i++] = ch;
    goto state_initial;

  default:
    buf[i] = '\0';
    ch = unget_ch(l);
    break;
  }

  switch (num_type) {
  case NUM_FLOAT:
  case NUM_DOUBLE:
    tok->value.Float = strtod(buf, &end);
    tok->kind = TK_NUMBER;
    break;

  default:
    tok->value.Integer = strtol(buf, &end, 0);
    tok->kind = TK_NUMBER;
    break;
  }

  if (*end != '\0') {
    /* TODO BAD FORMAT */
  }

  return ch;
}
#endif

static char strbuf[1024] = {'\0'};
static int ci = 0;

static int read_token(struct lexer *l, struct token *tok)
{
  char ch = '\0';

state_initial:
  ch = get_ch(l);

  switch (ch) {
  case ' ':
  case '\t':
  case '\v':
    goto state_initial;

  case '\n':
    detect_newline(l);
    goto state_initial;

  case '\0':
    tok->kind = TK_EOS;
    goto state_final;

  case '\'':
    ch = get_ch(l);
    if (get_ch(l) == '\'') {
      tok->kind = TK_NUMBER;
      tok->value.word[0] = ch;
      tok->value.word[1] = '\0';
      goto state_final;
    } else {
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '"':
    ci = 0;
    goto state_string_literal;

  case '+':
    ch = get_ch(l);
    switch (ch) {
    case '+':
      tok->kind = TK_INC;
      goto state_final;
    case '=':
      tok->kind = TK_ADD_ASSIGN;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '-':
    ch = get_ch(l);
    switch (ch) {
    case '-':
      tok->kind = TK_DEC;
      goto state_final;
    case '=':
      tok->kind = TK_SUB_ASSIGN;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '*':
    ch = get_ch(l);
    switch (ch) {
    case '=':
      tok->kind = TK_MUL_ASSIGN;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '/':
    ch = get_ch(l);
    switch (ch) {
    case '*':
      goto state_block_comment;
    case '/':
      goto state_line_comment;
    case '=':
      tok->kind = TK_DIV_ASSIGN;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '<':
    ch = get_ch(l);
    switch (ch) {
    case '=':
      tok->kind = TK_LE;
      goto state_final;
    case '<':
      tok->kind = TK_LSHIFT;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '>':
    ch = get_ch(l);
    switch (ch) {
    case '=':
      tok->kind = TK_GE;
      goto state_final;
    case '>':
      tok->kind = TK_RSHIFT;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '=':
    ch = get_ch(l);
    switch (ch) {
    case '=':
      tok->kind = TK_EQ;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '!':
    ch = get_ch(l);
    switch (ch) {
    case '=':
      tok->kind = TK_NE;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '&':
    ch = get_ch(l);
    switch (ch) {
    case '&':
      tok->kind = TK_AND;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '|':
    ch = get_ch(l);
    switch (ch) {
    case '|':
      tok->kind = TK_OR;
      goto state_final;
    default:
      ch = unget_ch(l);
      tok->kind = ch;
      goto state_final;
    }

  case '.':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    unget_ch(l);
    ch = scan_number(l, tok);
    goto state_final;

  case 'A': case 'B': case 'C': case 'D': case 'E':
  case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O':
  case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case 'a': case 'b': case 'c': case 'd': case 'e':
  case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o':
  case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    unget_ch(l);
    ch = scan_word(l, tok);
    goto state_final;

  default:
    tok->kind = ch;
    goto state_final;
  }

state_string_literal:
  ch = get_ch(l);
  switch (ch) {
  case '"':
    tok->kind = TK_STRING_LITERAL;
    strbuf[ci] = '\0';
    tok->value.String = strbuf;
    goto state_final;
  default:
    strbuf[ci++] = ch;
    goto state_string_literal;
  }

state_block_comment:
  ch = get_ch(l);
  switch (ch) {
  case '*':
    ch = get_ch(l);
    switch (ch) {
    case '/':
      goto state_initial;
    default:
      goto state_block_comment;
    }
  case '\0':
    /* TODO error handling */
    tok->kind = ch;
    goto state_final;
  case '\n':
    detect_newline(l);
    goto state_block_comment;
  default:
    goto state_block_comment;
  }

state_line_comment:
  ch = get_ch(l);
  switch (ch) {
  case '\n':
    detect_newline(l);
    goto state_initial;
  default:
    goto state_line_comment;
  }

state_final:
  return tok->kind;
}

int lex_input_string(struct lexer *l, const char *string)
{
  int err = 0;
  struct lexer ll = LEXER_INIT;

  *l = ll;
  err = open_string_stream(&l->strm, string);

  if (err)
    return -1;
  else
    return 0;
}

int lex_input_file(struct lexer *l, const char *filename)
{
  int err = 0;
  struct lexer ll = LEXER_INIT;

  *l = ll;
  err = open_file_stream(&l->strm, filename);

  if (err)
    return -1;
  else
    return 0;
}

void lex_finish(struct lexer *l)
{
  close_stream(&l->strm);
}

const struct token *lex_get_token(struct lexer *l)
{
  if (l->is_head) {
    struct token tok;
    read_token(l, &tok);
    fwd_tokbuf(l);
    save_tok(l, &tok);
  } else {
    l->is_head = 1;
    fwd_tokbuf(l);
  }

  return load_tok(l);
}

const struct token *lex_unget_token(struct lexer *l)
{
  if (l->is_head) {
    l->is_head = 0;
    bwd_tokbuf(l);
  }

  return load_tok(l);
}

const struct token *lex_current_token(const struct lexer *l)
{
  return load_tok(l);
}

int lex_get_line_num(const struct lexer *l)
{
  return l->line;
}

int lex_get_column_num(const struct lexer *l)
{
  return l->column;
}
