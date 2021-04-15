#ifndef __HEAP_PUBLIC_H__
#define __HEAP_PUBLIC_H__

#include "enum.h"
#include "error.h"

//Use the following naming convention for Heap
struct heap;

//Pass array that needs to be heapified, the size and the type of heap
struct heap* heap_init(int* array, int size, enum HEAP_TYPE type);

//Pass index of the node that needs to be deleted and it will also be extracted into 'val'
int delete_node(struct heap* hp, int index, int* val);

//Update the node at index with the value
int update_node(struct heap* hp, int index, int value);

//Add a new node
int add_node(struct heap* hp, int value);

//Just peek the top most element
int peek(struct heap* hp);

//Get the curr heap size
int get_heap_size(struct heap* hp);

//Heapsort INTO the 'array' that should be the same size as curr heap size
int heap_sort(struct heap* hp, int* array, int size);

//Destroy heap
int heap_destroy(struct heap* hp);

#endif
