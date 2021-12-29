#ifndef __HEAP_PUBLIC_H__
#define __HEAP_PUBLIC_H__

#include "enum.h"
#include "error.h"

//Use the following naming convention for Heap
struct heap;

//Pass array that needs to be heapified, the size and the type of heap
struct heap* heap_init(int* array, int size, enum HEAP_TYPE type);

//Add a new node
int add_node(struct heap* hp, int value);

//Update the node at index with the value
int update_node(struct heap* hp, int index, int value);

//Pass index of the node that needs to be deleted and it will also be extracted into 'val'
int delete_node(struct heap* hp, int index, int* val);

//Just peek the top most element
int peek(struct heap* hp);

//Get the curr heap size
int get_heap_size(struct heap* hp);

//Heapsort INTO the 'dst_array'...provided by Client that should be the same size or greater as curr heap size
int heap_sort(struct heap* hp, int* dst_array, int size); //Adding 'size' attribute so you are FORCED to think if its right

//Destroy heap
int heap_destroy(struct heap* hp);

#endif
