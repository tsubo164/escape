/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "ast.h"
#include "cgen.h"
#include "parser.h"
#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char **argv)
{
  const char *filename = NULL;
  struct ast_node *node = NULL;
  struct symbol_table *symtbl = NULL;
  struct parser p = PARSER_INIT;
  int print_c = 0;
  int print_tree = 0;
  FILE *fp = NULL;
  char cfile[128] = {'\0'};

  if (argc == 3 && strcmp(argv[1], "-p") == 0) {
    filename = argv[2];
    fp = stdout;
    print_c = 1;
  } else if (argc == 3 && strcmp(argv[1], "-t") == 0) {
    filename = argv[2];
    fp = stdout;
    print_tree = 1;
  } else if (argc == 2) {
    filename = argv[1];
    sprintf(cfile, "%s.c", filename);
    fp = fopen(cfile, "w");
  } else {
    return -1;
  }

  symtbl = new_symbol_table();
  p.symtbl = symtbl;

  node = parse_file(&p, filename);
  if (print_tree) {
    ast_print_tree(node);
  } else {
    struct context cxt = INIT_CONTEXT;
    print_c_code(fp, node, &cxt);
  }

  if (!print_c && !print_tree) {
    char cmd[128] = {'\0'};
    fclose(fp);

    sprintf(cmd, "cc %s", cfile);
    system(cmd);
    sprintf(cmd, "rm -f %s", cfile);
    system(cmd);
  }

  free_symbol_table(symtbl);

  ast_free_node(node);
  parse_finish(&p);
  return 0;
}
