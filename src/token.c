/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "token.h"
#include <string.h>
#include <ctype.h>

int kind_of(const struct token *tok)
{
  return tok->kind;
}

int int_value_of(const struct token *tok)
{
  return tok->value.Integer;
}

float float_value_of(const struct token *tok)
{
  return tok->value.Float;
}

const char *word_value_of(const struct token *tok)
{
  return tok->value.word;
}

const char *string_value_of(const struct token *tok)
{
  return tok->value.String;
}

static const char *ascii2str[] = {
"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT",
"LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3",
"DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS",
"RS", "US", "SPACE",
"!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",
"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
":", ";", "<", "=", ">", "?", "@",
"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
"[", "\\", "]", "^", "_", "`",
"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
"{", "|", "}", "~", "DEL"};

static const struct tks{
  int kind;
  const char *str;
} tokstr[] = {
#define T(tag,str) {tag,str},
  TOKEN_KIND_LIST(T)
  KEYWORD_LIST(T)
#undef T
  {0,""} /* for no-comma entry */
};
static const size_t N_TOKENS = sizeof(tokstr)/sizeof(struct tks) - 1;

const char *kind_to_string(int kind)
{
  int i;

  if (kind < 128) {
    return ascii2str[kind];
  }

  for (i = 0; i < N_TOKENS; i++) {
    if (tokstr[i].kind == kind) {
      return tokstr[i].str;
    }
  }

  return "";
}
