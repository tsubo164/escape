/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef DATATYPE_H
#define DATATYPE_H

enum {
  TYPE_NONE,
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING
};

typedef int DataType;

extern const char *DataType_String(int datatype);

#endif /* XXX_H */

