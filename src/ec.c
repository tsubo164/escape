/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "parser.h"
#include "cgen.h"
#include "ast.h"
#include <stdio.h>

int main(int argc, const char **argv)
{
  const char *filename = NULL;
  struct parser p = PARSER_INIT;
  struct ast_node *node = NULL;

  if (argc != 2) {
    return -1;
  }
  filename = argv[1];

  node = parse_file(&p, filename);
  if (node != NULL) {
    struct context cxt = INIT_CONTEXT;
    print_c_code(node, &cxt);
  }

  ast_free_node(node);
  parse_finish(&p);
  return 0;
}
