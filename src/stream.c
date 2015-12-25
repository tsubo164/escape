/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "stream.h"
#include "memory.h"
#include <string.h>

static int get_ch(struct stream *s)
{
  switch (s->type) {
  case STREAM_FILE:
    return fgetc(s->file);
  case STREAM_STRING:
    return s->text[s->text_i];
  default:
    return 0;
  }
}

static void store_ch(struct stream *s, char c)
{
  s->bucket_i = (s->bucket_i + 1) % BUCKET_SIZE;
  s->bucket[s->bucket_i] = c;
  s->text_i++;
}

static char fwd_ch(struct stream *s)
{
  char c = '\0';
  s->bucket_i = (s->bucket_i + 1) % BUCKET_SIZE;
  c = s->bucket[s->bucket_i];
  s->behind--;
  return c;
}

static char bwd_ch(struct stream *s)
{
  char c = '\0';
  s->bucket_i = (s->bucket_i - 1 + BUCKET_SIZE) % BUCKET_SIZE;
  c = s->bucket[s->bucket_i];
  s->behind++;
  return c;
}

int open_string_stream(struct stream *s, const char *string)
{
  struct stream strm = STREAM_INIT;
  const size_t len = strlen(string);

  strm.type = STREAM_STRING;
  strm.text = MEMORY_ALLOC_ARRAY(char, len + 1);
  strncpy(strm.text, string, len + 1);

  *s = strm;
  return 0;
}

int open_file_stream(struct stream *s, const char *filename)
{

  struct stream strm = STREAM_INIT;

  strm.type = STREAM_FILE;
  strm.file = fopen(filename, "r");

  if (strm.file == NULL)
    return -1;

  *s = strm;
  return 0;
}

void close_stream(struct stream *s)
{
  if (s->file != NULL)
    fclose(s->file);

  if (s->text != NULL)
    MEMORY_FREE(s->text);
}

char stream_getc(struct stream *s)
{
  int c = '\0';

  if (s->behind > 0) {
    c = fwd_ch(s);
    return (char) c;
  }

  c = get_ch(s);

  if (c > 0) {
    store_ch(s, c);
    return (char) c;
  } else {
    return '\0';
  }
}

char stream_ungetc(struct stream *s)
{
  return bwd_ch(s);
}
