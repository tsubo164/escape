/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "ast.h"
#include "cgen.h"
#include "parser.h"
#include "symbol.h"
#include <stdio.h>

int main(int argc, const char **argv)
{
  const char *filename = NULL;
  struct symbol_table *symtbl = NULL;
  struct parser p = PARSER_INIT;
  struct ast_node *node = NULL;

  if (argc != 2) {
    return -1;
  }
  filename = argv[1];

  symtbl = new_symbol_table();
  p.symtbl = symtbl;

  node = parse_file(&p, filename);
  if (node != NULL) {
    struct context cxt = INIT_CONTEXT;
    print_c_code(node, &cxt);
  }

  free_symbol_table(symtbl);

  ast_free_node(node);
  parse_finish(&p);
  return 0;
}
