/* Copyright 2010 Nick Johnson */

#include <flux/heap.h>

#include <stdlib.h>

void free(void *ptr) {
	heap_free(ptr);
}