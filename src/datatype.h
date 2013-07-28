/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef DATATYPE_H
#define DATATYPE_H

enum {
  TYPE_NONE,
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_SHORT,
  TYPE_INT,
  TYPE_LONG,
  TYPE_FLOAT,
  TYPE_DOUBLE,
  TYPE_STRING
};

typedef int DataType;

extern const char *DataType_String(int datatype);

#endif /* XXX_H */

