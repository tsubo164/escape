/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef CGEN_H
#define CGEN_H

#include <stdio.h>

struct context {
  int depth;
  int is_inside_enum_def;
  int is_inside_initializer;
};
#define INIT_CONTEXT {0, 0, 0};

struct ast_node;

extern void print_c_code(FILE *fp, const struct ast_node *node, struct context *cxt);

#endif /* XXX_H */
