/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "type.h"
#include <stdlib.h>

static const char *type_table[] = {
#define T(kind,str) str,
  TYPE_KIND_LIST(T)
#undef T
  "" /* for no-comma entry */
};
static const size_t N_TYPES = sizeof(type_table)/sizeof(type_table[0]) - 1;

const char *type_to_string(int type)
{
  if (type < 0 || type >= N_TYPES) {
    return "";
  }
  return type_table[type];
}
