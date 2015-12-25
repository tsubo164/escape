/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef CCODE_H
#define CCODE_H

#include <stdio.h>

struct context {
  int depth;
};
#define INIT_CONTEXT {0};

struct ast_node;

extern void print_c_code(FILE *fp, const struct ast_node *node, struct context *cxt);

#endif /* XXX_H */
