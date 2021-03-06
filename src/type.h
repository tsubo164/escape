/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef TYPE_H
#define TYPE_H

#include <stdlib.h>

#define TYPE_KIND_LIST(T) \
  T(TYPE_UNKNOWN, "unknown") \
  T(TYPE_BOOL, "bool") \
  T(TYPE_CHAR, "char") \
  T(TYPE_SHORT, "short") \
  T(TYPE_INT, "int") \
  T(TYPE_LONG, "long") \
  T(TYPE_FLOAT, "float") \
  T(TYPE_DOUBLE, "double") \
  T(TYPE_STRING, "string") \
  T(TYPE_VOID, "void")

enum type_kind {
#define T(tag,str) tag,
  TYPE_KIND_LIST(T)
#undef T
  TYPE_NONE /* for no-comma entry */
};

struct type_info {
  char kind;
  char is_array;
  size_t array_size;
};

#define INIT_TYPE_INFO {TYPE_UNKNOWN,0,0}

extern const char *type_to_string(int type);

#endif /* XXX_H */
