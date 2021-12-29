#ifndef __HEAP_PRIVATE_H__
#define __HEAP_PRIVATE_H__

#include <stdbool.h>
#include <pthread.h>

#include "enum.h"

struct heap {

    //User provided values
    int* array; //Un-orderdered array that needs to be heapified
    int max_size;
#define MAX_ELEMENTS 0
    int max_elem; //This is the max size of array that we will allow. It equals MAX_ELEMENTS
    enum HEAP_TYPE type;

    //attributes used for the Heap library - not user provided
    struct heap* obj_id;
    bool init;
    bool is_heapify_run;
    pthread_mutex_t lock_heap;	//lock heap operations i.e. add, delete, update
    int curr_size;

/*
.
. Why is Heapsort attribute not a part of this structure?
. The idea is to NOT use heapsort on an existing Heap, since it is in-place.
. We need to create a memcpy of the current heapified array and pass it to
. the sorting algorithm provided by this Heap library. The reason we will NOT
. heapsort the current array [ which is part of the struct heap ] is because
. IF we do that, it will NO longer be a Heap and this destroys the purpose of
. the whole structure. So heapsort will be used on an independent array that
. is heapified BUT not part of this structure. 
.
*/

};

#endif
