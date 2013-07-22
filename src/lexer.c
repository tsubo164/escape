/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "lexer.h"
#include "datatype.h"
#include "memory.h"

#include <string.h>
#include <ctype.h>

#define isidentifier(c) (isalnum((c)) || (c)=='_')

static char scan_name(struct Lexer *lexer, struct Token *token);
static char scan_number(struct Lexer *lexer, struct Token *token);

struct StringBuffer {
  char *c;
  int alloc;
  int len;
};

static struct StringBuffer *StringBuffer_New(void)
{
  struct StringBuffer *strbuf = NULL;

  strbuf = MEMORY_ALLOC(struct StringBuffer);
  if (strbuf == NULL) {
    return NULL;
  }

  strbuf->c = NULL;
  strbuf->alloc = 0;
  strbuf->len = 0;

  return strbuf;
}

static void StringBuffer_AppendChar(struct StringBuffer *strbuf, char c)
{
  if (strbuf->len + 1 >= strbuf->alloc) {
    const int new_alloc = strbuf->alloc == 0 ? 128 : 2 * strbuf->alloc;
    strbuf->c = MEMORY_REALLOC_ARRAY(strbuf->c, char, new_alloc);
  }
  strbuf->c[strbuf->len] = c;
  strbuf->len++;
  strbuf->c[strbuf->len] = '\0';
}

static void StringBuffer_Clear(struct StringBuffer *strbuf)
{
  if (strbuf->c == NULL) {
    return;
  }
  strbuf->len = 0;
  strbuf->c[0] = '\0';
}

static void StringBuffer_Free(struct StringBuffer *strbuf)
{
  if (strbuf == NULL) {
    return;
  }
  MEMORY_FREE(strbuf->c);
  MEMORY_FREE(strbuf);
}

struct Keyword {
  const char *name;
  int token_tag;
  int data_type;
};

/* TODO should be decoupled from datatype? */
/* sorted by alphabet */
static const struct Keyword keywords[] = {
  {"break",    TK_BREAK,    TYPE_NONE},
  {"case",     TK_CASE,     TYPE_NONE},
  {"continue", TK_CONTINUE, TYPE_NONE},
  {"default",  TK_DEFAULT,  TYPE_NONE},
  {"do",       TK_DO,       TYPE_NONE},
  {"else",     TK_ELSE,     TYPE_NONE},
  {"float",    TK_FLOAT,    TYPE_FLOAT},
  {"for",      TK_FOR,      TYPE_NONE},
  {"function", TK_FUNCTION, TYPE_NONE},
  {"goto",     TK_GOTO,     TYPE_NONE},
  {"if",       TK_IF,       TYPE_NONE},
  {"int",      TK_INT,      TYPE_INT},
  {"label",    TK_LABEL,    TYPE_NONE},
  {"return",   TK_RETURN,   TYPE_NONE},
  {"string",   TK_STRING,   TYPE_STRING},
  {"switch",   TK_SWITCH,   TYPE_NONE},
  {"var",      TK_VAR,      TYPE_NONE},
  {"vardump",  TK_VARDUMP,  TYPE_NONE},
  {"while",    TK_WHILE,    TYPE_NONE}
};
static const size_t N_KEYWORDS = sizeof(keywords)/sizeof(keywords[0]);
static int compare_keywords(const void *ptr0, const void *ptr1);
static void keyword_or_identifier(struct Token *token);

enum {
  BUFFER_SIZE = 1024
};
enum {
  INPUT_FILE = 0,
  INPUT_STRING
};

static void detect_new_line(struct Lexer *lexer);
static char get_next_char(struct Lexer *lexer);
static char put_back_char(struct Lexer *lexer);

struct Lexer {
  int line_number;
  int input_type;

  char *buffer;
  int nalloc;

  const char *current_char;
  FILE *input_file;

  struct StringBuffer *string_literal;
};

struct Lexer *Lexer_New(void)
{
  struct Lexer *lexer = NULL;

  lexer = MEMORY_ALLOC(struct Lexer);
  if (lexer == NULL) {
    return NULL;
  }

  lexer->input_type = INPUT_FILE;
  lexer->line_number = 1;

  lexer->buffer = NULL;
  lexer->nalloc = 0;

  lexer->current_char = NULL;
  lexer->input_file = NULL;

  lexer->string_literal = StringBuffer_New();

  return lexer;
}

void Lexer_Free(struct Lexer *lexer)
{
  if (lexer == NULL) {
    return;
  }
  MEMORY_FREE(lexer->buffer);

  StringBuffer_Free(lexer->string_literal);

  MEMORY_FREE(lexer);
}

int Lexer_NextToken(struct Lexer *lexer, struct Token *token)
{
  char ch = '\0';

state_initial:
  ch = get_next_char(lexer);
  switch (ch) {
  case ' ':
  case '\t':
  case '\v':
    goto state_initial;

  case '\n':
    detect_new_line(lexer);
    goto state_initial;

  case '\0':
    token->tag = ch;
    goto state_final;

  case '"':
    StringBuffer_Clear(lexer->string_literal);
    goto state_string_literal;

  case '/':
    ch = get_next_char(lexer);
    switch (ch) {
    case '*':
      goto state_block_comment;
    case '/':
      goto state_line_comment;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '<':
    ch = get_next_char(lexer);
    switch (ch) {
    case '=':
      token->tag = TK_LE;
      goto state_final;
    case '<':
      token->tag = TK_LSHIFT;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '>':
    ch = get_next_char(lexer);
    switch (ch) {
    case '=':
      token->tag = TK_GE;
      goto state_final;
    case '>':
      token->tag = TK_RSHIFT;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '=':
    ch = get_next_char(lexer);
    switch (ch) {
    case '=':
      token->tag = TK_EQ;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '!':
    ch = get_next_char(lexer);
    switch (ch) {
    case '=':
      token->tag = TK_NE;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '&':
    ch = get_next_char(lexer);
    switch (ch) {
    case '&':
      token->tag = TK_AND;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '|':
    ch = get_next_char(lexer);
    switch (ch) {
    case '|':
      token->tag = TK_OR;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '+':
    ch = get_next_char(lexer);
    switch (ch) {
    case '+':
      token->tag = TK_INC;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '-':
    ch = get_next_char(lexer);
    switch (ch) {
    case '-':
      token->tag = TK_DEC;
      goto state_final;
    default:
      ch = put_back_char(lexer);
      token->tag = ch;
      goto state_final;
    }

  case '.':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  /*
    ch = scan_number(lexer, token);
  */
    put_back_char(lexer);
    ch = scan_number(lexer, token);
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
    ch = scan_name(lexer, token);
    goto state_final;

  default:
    token->tag = ch;
    goto state_final;
  }

state_string_literal:
  ch = get_next_char(lexer);
  switch (ch) {
  case '"':
    token->tag = TK_STRING_LITERAL;
    token->value.string = lexer->string_literal->c;
    goto state_final;
  default:
    StringBuffer_AppendChar(lexer->string_literal, ch);
    goto state_string_literal;
  }

state_block_comment:
  ch = get_next_char(lexer);
  switch (ch) {
  case '*':
    ch = get_next_char(lexer);
    switch (ch) {
    case '/':
      /*
      token->tag = TK_COMMENT;
      goto state_final;
      */
      goto state_initial;
    default:
      goto state_block_comment;
    }

  case '\n':
    detect_new_line(lexer);
    goto state_block_comment;

  default:
    goto state_block_comment;
  }

state_line_comment:
  ch = get_next_char(lexer);
  switch (ch) {
  case '\n':
    /*
    token->tag = TK_COMMENT;
    detect_new_line(lexer);
    goto state_final;
    */
    detect_new_line(lexer);
    goto state_initial;

  default:
    goto state_line_comment;
  }

state_final:
  return 0;
}

int Lexer_GetLineNumber(const struct Lexer *lexer)
{
  return lexer->line_number;
}

void Lexer_SetInputFile(struct Lexer *lexer, FILE *file)
{
  char *next_char = NULL;
  int max_size = 0;

  lexer->nalloc = BUFFER_SIZE;
  lexer->buffer = MEMORY_ALLOC_ARRAY(char, BUFFER_SIZE);
  if (lexer->buffer == NULL) {
    /* TODO error handling */
  }

  lexer->input_file = file;
  next_char = lexer->buffer + 1;
  max_size = BUFFER_SIZE - 1;

  lexer->buffer[0] = '\0';
  lexer->buffer[1] = '\0';

  if (fgets(next_char, max_size, lexer->input_file) == NULL) {
    /* TODO */
  }

  lexer->current_char = lexer->buffer;
  lexer->line_number = 1;
  lexer->input_type = INPUT_FILE;
}

void Lexer_SetInputString(struct Lexer *lexer, const char *str)
{
  const size_t alloc_size = strlen(str)
      + 1  /* for null char at end */
      + 1; /* for the previous char from input */

  lexer->nalloc = alloc_size;
  lexer->buffer = MEMORY_ALLOC_ARRAY(char, alloc_size);
  if (lexer->buffer == NULL) {
    /* TODO error handling */
  }
  lexer->buffer[0] = '\0';

  strncpy(lexer->buffer + 1, str, alloc_size - 1);

  lexer->current_char = lexer->buffer;
  lexer->line_number = 1;
  lexer->input_type = INPUT_STRING;
}

static void detect_new_line(struct Lexer *lexer)
{
  char *next_char = NULL;
  int max_size = 0;

  lexer->line_number++;

  if (lexer->input_type == INPUT_STRING) {
    return;
  }

  if (lexer->input_file == NULL) {
    return;
  }

  next_char = lexer->buffer + 1;
  max_size = BUFFER_SIZE - 1;

  lexer->buffer[0] = '\0';
  lexer->buffer[1] = '\0';

  if (fgets(next_char, max_size, lexer->input_file) == NULL) {
    return;
  }

  lexer->current_char = lexer->buffer;
}

static char get_next_char(struct Lexer *lexer)
{
  if (lexer->current_char < lexer->buffer + lexer->nalloc - 1) {
    lexer->current_char++;
  }
  return *(lexer->current_char);
}

static char put_back_char(struct Lexer *lexer)
{
  if (lexer->current_char > lexer->buffer) {
    lexer->current_char--;
  }
  return *(lexer->current_char);
}

static char scan_name(struct Lexer *lexer, struct Token *token)
{
  const char *src = lexer->current_char;
  char *dst = token->value.name;
  const char *end = dst + MAX_NAME_SIZE - 1;

  while (*src != '\0' && dst < end) {
    if (isidentifier(*src)) {
      *dst++ = *src++;
      continue;
    } else {
      break;
    }
  }
  *dst = '\0';

  keyword_or_identifier(token);

  lexer->current_char = src - 1;

  return (*lexer->current_char);
}

static char scan_number(struct Lexer *lexer, struct Token *token)
{
  char buf[265] = {'\0'};
  char ch = '\0';
  char *end = NULL;
  int i = 0;
  int is_float = 0;

state_initial:
  ch = get_next_char(lexer);

  switch (ch) {
  case '+': case '-':
    if (i > 0) {
      is_float = 1;
    }
    buf[i++] = ch;
    goto state_initial;

  case '.':
  case 'e': case 'E':
  case 'f': case 'F':
    is_float = 1;
    buf[i++] = ch;
    goto state_initial;

  case 'x': case 'X':
  case 'u': case 'U':
  case 'l': case 'L':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    buf[i++] = ch;
    goto state_initial;

  default:
    buf[i] = '\0';
    ch = put_back_char(lexer);
    break;
  }

  if (is_float) {
    token->value.number = strtod(buf, &end);
    token->tag = TK_NUMBER;
    /*
    printf("%c, %c\n", ch, *(--end));
    */
  } else {
    token->value.number = strtol(buf, &end, 0);
    token->tag = TK_NUMBER;
  }

  if (*end != '\0') {
    /* TODO BAD FORMAT */
  }

  return (*lexer->current_char);
}

#if 0
typedef struct Digit {
  long value;
  int ndigits;
} Digit;
static Digit scan_digits(struct Lexer *lexer)
{
  Digit digits = {0, 0};

  for (;;) {
    char ch = get_next_char(lexer);
    if (!isdigit(ch)) {
      put_back_char(lexer);
      break;
    }
    digits.value = 10 * digits.value + (ch - '0');
    digits.ndigits++;
  }

  return digits;
}

static double power(int x, int y)
{
  double p = 1;
  int i;

  if (y >= 0) {
    for (i = 0; i < y; i++) {
      p *= x;
    }
  } else {
    for (i = 0; i < -y; i++) {
      p *= x;
    }
    p = 1./p;
  }

  return p;
}

static char scan_number(struct Lexer *lexer, struct Token *token)
{
  Digit integer = {0, 0};
  Digit decimal = {0, 0};
  Digit exponent = {0, 0};
  int is_frac = 0;
  int is_expo = 0;
  int e_sign = 1;
  char ch = '\0';

  integer = scan_digits(lexer);

state_initial:
  ch = get_next_char(lexer);

  switch (ch) {
  case '.':
    is_frac = 1;
    decimal = scan_digits(lexer);
    goto state_initial;

  case 'e': case 'E':
    ch = get_next_char(lexer);
    switch (ch) {
    case '-':
      e_sign = -1;
      break;
    case '+':
      e_sign = 1;
      break;
    default:
      put_back_char(lexer);
      e_sign = 1;
      break;
    }
    is_expo = 1;
    exponent = scan_digits(lexer);
    goto state_initial;

  case 'f': case 'F':
    is_frac = 1;
    break;

  default:
    put_back_char(lexer);
    break;
  }

  if (is_frac) {
    token->value.number = integer.value + decimal.value/power(10, decimal.ndigits);
    token->tag = TK_NUMBER;
  } else {
    token->value.number = (double) integer.value;
    token->tag = TK_NUMBER;
  }

  if (is_expo) {
    token->value.number *= power(10, e_sign * exponent.value);
  }

  return (*lexer->current_char);
}
#endif

static void keyword_or_identifier(struct Token *token)
{
  const struct Keyword *found = NULL;
  struct Keyword key;
  key.name = token->value.name;

  found = bsearch(&key, keywords, N_KEYWORDS, sizeof(keywords[0]), compare_keywords);

  if (found) {
    token->tag       = found->token_tag;
    token->data_type = found->data_type;
  } else {
    token->tag = TK_IDENTIFIER;
  }
}

static int compare_keywords(const void *ptr0, const void *ptr1)
{
  const struct Keyword *kw0 = (const struct Keyword *) ptr0;
  const struct Keyword *kw1 = (const struct Keyword *) ptr1;
  return strcmp(kw0->name, kw1->name);
}

