/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef CCODE_H
#define CCODE_H

struct context {
  int depth;
};
#define INIT_CONTEXT {0};

struct ast_node;

extern void print_c_code(const struct ast_node *node, struct context *cxt);

#endif /* XXX_H */
