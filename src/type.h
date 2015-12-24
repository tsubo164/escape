/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef TYPE_H
#define TYPE_H

#define TYPE_KIND_LIST(T) \
  T(TYPE_VOID, "void") \
  T(TYPE_INT, "int") \
  T(TYPE_STRING, "string")

enum type_kind {
#define T(tag,str) tag,
  TYPE_KIND_LIST(T)
#undef T
  TYPE_NONE
};

extern const char *type_to_string(int type);

#endif /* XXX_H */
