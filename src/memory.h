/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

/* force type to avoid missing sizeof mulptiplication */
#define MEM_ALLOC_ARRAY(type,nelems) ((type*) malloc(sizeof(type) * (nelems)))

/* for single object */
#define MEM_ALLOC(type) (MEM_ALLOC_ARRAY(type, 1))

#define MEM_REALLOC_ARRAY(ptr,type,nelems) ((type*) realloc((ptr), sizeof(type) * (nelems)))

#define MEM_FREE(ptr) (free((ptr)))

#endif /* XXX_H */
