#ifndef __HEAP_PUBLIC_H__
#define __HEAP_PUBLIC_H__

#include "error.h"

enum HEAP_TYPE {
    MIN_HEAP = 0,
    MAX_HEAP
};

struct heap* heap_init(int* array, int size, enum HEAP_TYPE type);

int delete_node(struct heap* hp, int index);

int update_node(struct heap* hp, int index, int value);

int add_node(struct heap* hp, int value);

int peek(struct heap* hp);

int get_heap_size(struct heap* hp);

int heap_destroy(struct heap* hp);

#endif
