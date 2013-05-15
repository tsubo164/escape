/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "datatype.h"

struct DataTypeString {
	int datatype;
	const char *datastr;
};

const struct DataTypeString datatype_strings[] = {
	{TYPE_NONE,   "none"},
	{TYPE_INT,    "int"},
	{TYPE_STRING, "string"}
};
static const int N_STRINGS = sizeof(datatype_strings)/sizeof(datatype_strings[0]);

const char *DataType_String(int datatype)
{
	int i;
	for (i = 0; i < N_STRINGS; i++) {
		if (datatype_strings[i].datatype == datatype) {
			break;
		}
	}
	return datatype_strings[i].datastr;
}

