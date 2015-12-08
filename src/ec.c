/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include <stdio.h>
#include "parser.h"

int main(int argc, const char **argv)
{
  const char *filename = NULL;
  struct parser p = PARSER_INIT;

  if (argc != 2) {
    return -1;
  }

  filename = argv[1];

  parse_file(&p, filename);

  parse_finish(&p);

  return 0;
}
