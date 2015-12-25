/*
Copyright (c) 2012-2015 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

/* force type to avoid missing sizeof mulptiplication */
#define MEMORY_ALLOC_ARRAY(type,nelems) ((type*) malloc(sizeof(type) * (nelems)))

/* for single object */
#define MEMORY_ALLOC(type) (MEMORY_ALLOC_ARRAY(type, 1))

#define MEMORY_REALLOC_ARRAY(ptr,type,nelems) ((type*) realloc((ptr), sizeof(type) * (nelems)))

#define MEMORY_FREE(ptr) (free((ptr)))

#endif /* XXX_H */
