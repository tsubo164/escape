/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

#define BUCKET_SIZE 32

enum {
  STREAM_NONE = 0,
  STREAM_STRING,
  STREAM_FILE
};

struct stream {
  int type;

  char *text;
  int text_i;
  FILE *file;

  char bucket[BUCKET_SIZE];
  int bucket_i;
  int behind;
};

#define STREAM_INIT {STREAM_NONE,NULL,0,NULL,{0},0,0}

extern int open_string_stream(struct stream *s, const char *string);
extern int open_file_stream(struct stream *s, const char *filename);
extern void close_stream(struct stream *s);

extern char stream_getc(struct stream *s);
extern char stream_ungetc(struct stream *s);

#endif /* XXX_H */
