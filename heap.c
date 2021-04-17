/****************************************************************************************************************************************************
. GENERAL INFORMATION:
.
. Whenever you get a problem that talks about the largest or smallest of something, try to think of Heap.
.
*****************************************************************************************************************************************************/

//C lib headers
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Private headers
#include "heap_private.h"

//Public headers
#include "heap_public.h"

int self_balancing_bst_insert(struct heap* hp) {

    int* data = hp->array;
    int size = hp->curr_size;

//HEAP
    int base = 0; //Base node
    int level = 0;
    int count = 5;
    while(base < size) {
       for(int i = 0; i < count; i++) printf(" ");
       int lvl_cnt = pow(2, level++);
       int l = 0;
       while(l < lvl_cnt) {
           if(base + l >= size) goto exit;
           printf("%d ", data[base + l++]);
       }      
       base = base + lvl_cnt;
       count--;
       printf("\n");
    }

exit:
    printf("\n\n");
   return E_EOK;
}

/****************************************************************************************************************************************************
.
. CLRS => "Good implementations of heapsort often implement these* procedures as “macros” or “in-line” procedures."
.
. these* refer to LEFT_CHILD, RIGHT_CHILD and PARENT
.
. CLRS => "HEAPIFY assumes that the binary trees rooted at LEFT[i] and RIGHT[i] are heaps (max or min), but that A[i]􏰀 might be larger or smaller than 
.          its children (depending on whether it is Max or Min Heap), thus violating the max-heap property."
.
. Not Synchronized, no locking
.
. 'heapify' is a top down algorithm. The 'parent_id' is considered the ROOT of the Heap and also the only violation that needs to be fixed. That node
. i.e. the 'parent_id' can either sink down or stay where it is.
.
. 'sift_up' is a bottom up algorithm. During 'update' the violation that is inserted can be such that the upper half is NO longer a Heap. If that is
. the case we bubble the element upwards in contrast to 'heapify'.
.
****************************************************************************************************************************************************/

#define LEFT_CHILD(node)	2*node + 1
#define RIGHT_CHILD(node)	2*node + 2
#define PARENT(node)		(node - 1)/2

//If 'type' is Invalid, it defaults to MAX_HEAP
static bool 
compare(int* array, int node1, int node2, int type) {

    switch (type) {

        case MIN_HEAP:
            return ((array[node1] < array[node2]));    
        break; //redundant

        case MAX_HEAP:
        default:
            return ((array[node1] > array[node2]));    
        }
}

/*
.
. HEAPIFY => This can fix a SINGLE VIOLATION of the Heap property in one iteration. So when you input an un-ordered Array, we go from
. 'n/2' to 0 because 'n/2 + 1 -> n' are all children which are already Heaps. For each iteration from 'n/2' to 0 we run the Heapify, and we keep
. fixing these single violations. 
. 
. This is how we build a Heap i.e. Top Down approach because for each iteration we assume that the Left and Right Sub-trees are Heaps and we compare with 
. those and let the violation float down to the proper place.
.
*/

static void 
heapify(int* array, int size, int parent_id, enum HEAP_TYPE type) {

//algo
    int l_child_id  = LEFT_CHILD(parent_id);
    int r_child_id  = RIGHT_CHILD(parent_id);

    int res_id = parent_id; //result id

    if((l_child_id < size) && compare(array, l_child_id, res_id, type)) {
        res_id = l_child_id;
    }

    if((r_child_id < size) && compare(array, r_child_id, res_id, type)) {
        res_id = r_child_id;
    }

    if(res_id != parent_id) {
        //Swap both
        int temp = array[res_id];
        array[res_id] = array[parent_id];
        array[parent_id] = temp;

        heapify(array, size, res_id, type);
    }
}

static void
heapify_wrapper(struct heap* hp, int parent_id) {

    int* array = hp->array;
    int size = hp->curr_size;
    enum HEAP_TYPE type = hp->type;

    heapify(array, size, parent_id, type);
}

static void
sift_up(struct heap* hp, int index) {

    int* array = hp->array;
    enum HEAP_TYPE type = hp->type;

    while(index != 0) {
        if(compare(array, index, PARENT(index), type)) {
            int temp = array[index];
            array[index] = array[PARENT(index)];
            array[PARENT(index)] = temp;
        } else {
            break;
        }

        index = PARENT(index);
    }
}

/****************************************************************************************************************************************************
.
. Initializing the Heap with 3 inputs. Array, size and type.
.
. We will run Heapify. Init takes O(n) time.
.
. Init creates a Malloc'd space and copies the original array there. It does NOT modify the original array
.
****************************************************************************************************************************************************/

static bool 
self_assert(struct heap* hp) {

#define ERROR	"\nError:   (self_assert) => "

    if(hp == NULL) {
        fprintf(stderr, "%sHeap pointer cannot be NULL", ERROR);
        return false;
    }

    if(!hp->init) {
        fprintf(stderr, "%sHeap structure [%p] init not done", ERROR, hp);
        return false;
    }

    return true;

#undef ERROR
}

//Provided array size is actually = 'size' attribute and it does not OOB [ or seg fault ]
struct heap* 
heap_init(int* array, int size, enum HEAP_TYPE type) {

#define ERROR	"\nError:   (heap_init) => "
#define WARNING	"\nWarning: (heap_init) => "

    if(array == NULL) {
        fprintf(stderr, "%sArray Data cannot be NULL", ERROR); 
        return NULL;
    }

    if(size <= 0) {
        fprintf(stderr, "%sSize of Array [%d] cannot be less than 1", ERROR, size); 
        return NULL;
    }

    if((type != MIN_HEAP) && (type != MAX_HEAP)) {
        fprintf(stderr, "%sHeap type [%d] not correct, defaulting to Max Heap", WARNING, type); 
    }

    struct heap* hp = (struct heap*)calloc(1, sizeof(struct heap)); //Init structure object
    if(hp == NULL) {
        fprintf(stderr, "%sCalloc failed", ERROR); 
        perror("Error");
        return NULL;
    }

    hp->obj_id = hp;

    if((size > MAX_ELEMENTS) && (MAX_ELEMENTS != 0)) {
        fprintf(stderr, "%sInput size [%d] greater than Heap max allowed size [%d] so defaulting to max", WARNING, size, MAX_ELEMENTS); 
        size = MAX_ELEMENTS;
    }

    hp->array = (int*)calloc(1, size*sizeof(int));
    if(hp->array == NULL) {
        fprintf(stderr, "%sCalloc failed", ERROR); 
        perror("Error");
        free(hp);	//Before any error return, free/unlock ALL objects in use
        return NULL;
    }

    memcpy(hp->array, array, size*sizeof(int));

    hp->max_size = size; //Changes if you resize the array [ usually happens during add_node ]
    hp->max_elem = MAX_ELEMENTS;
    hp->type = type;

    hp->curr_size = size;
    pthread_mutex_init(&hp->lock_heap, NULL);

    for(int i = size/2; i >=0; i--) {
        heapify_wrapper(hp, i);
    }

    hp->is_heapify_run = true;
    hp->init = true;

    fprintf(stderr, "%sHeap Init Success. Obj ID [%p] | Type: [%d] | Max Size: [%d] | Current Size: [%d] | Max Elements: [%d]", WARNING, hp, type, size, size, MAX_ELEMENTS); 

    return hp;

#undef ERROR
#undef WARNING
}

/****************************************************************************************************************************************************
.
. Delete node at 'index'
.
. Reduce 'curr_size' by 1
.
. Copy the last element to the index
.
. If the last element is at index then the bottom half is NOT a Heap anymore, BUT the top half is a heap, so we need to sink the element down
.
. For Sinking the element we use Heapify i.e. try to fit in somewhere in the Tree below. HOWEVER
.
. If a Leaf Node is deleted there is a possibility that the top half is NOT a heap, in such cases we 'sift_up'
.
. O(logn) time for this delete operation
.
. Can also be used to extract the root and it will put the value in 'val'
.
****************************************************************************************************************************************************/

int 
delete_node(struct heap* hp, int index, int* val) {

#define ERROR	"\nError:   (delete_node) => "

    if(!self_assert(hp)) return E_INIT;

    pthread_mutex_lock(&(hp->lock_heap));

    //If all elements in the array are deleted
    if(hp->curr_size < 1) {
        fprintf(stderr, "%sNo elements in the array, size [%d]", ERROR, hp->curr_size);
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_INVAL_PARAM;
    }

    //'index' must be valid
    if((index < 0) || (index >= hp->curr_size)) {
        fprintf(stderr, "%sIndex is invalid [%d]", ERROR, index);
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_INVAL_PARAM;
    }

    int* array = hp->array;
    int parent_id = PARENT(index);

    //prevent a seg fault
    if(val != NULL) {
        *val = array[index];
    }

    array[index] = array[hp->curr_size-- - 1];

    //sift-up -> opposite of heapify
    if(((hp->type == MAX_HEAP) && (array[parent_id] < array[index])) || ((hp->type == MIN_HEAP) && (array[parent_id] > array[index]))) {
        sift_up(hp, index);   
    } else {
        heapify_wrapper(hp, index);
    }

    pthread_mutex_unlock(&(hp->lock_heap));

    return E_EOK;

#undef ERROR
}

/****************************************************************************************************************************************************
.
.
****************************************************************************************************************************************************/

int 
update_node(struct heap* hp, int index, int value) {

#define ERROR	"\nError:   (update_node) => "

    if(!self_assert(hp)) return E_INIT;

    pthread_mutex_lock(&(hp->lock_heap));

    //'index' must be valid
    if((index < 0) || (index >= hp->curr_size)) {
        fprintf(stderr, "%sIndex is invalid [%d]", ERROR, index);
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_INVAL_PARAM;
    }

    int* array = hp->array;
    int parent_id = PARENT(index);

    array[index] = value;

    //sift-up -> opposite of heapify
    if(((hp->type == MAX_HEAP) && (array[parent_id] < array[index])) || ((hp->type == MIN_HEAP) && (array[parent_id] > array[index]))) {
        sift_up(hp, index);   
    } else {
        heapify_wrapper(hp, index);
    }
    
    pthread_mutex_unlock(&(hp->lock_heap));

    return E_EOK;

#undef ERROR
}

/****************************************************************************************************************************************************
.
.
****************************************************************************************************************************************************/

int 
add_node(struct heap* hp, int value) {

#define ERROR	"\nError:   (add_node) => "
#define WARNING	"\nWarning: (add_node) => "

    if(!self_assert(hp)) return E_INIT;
    
    pthread_mutex_lock(&(hp->lock_heap));

    //Realloc logic
    if(hp->curr_size == hp->max_size) {

        int new_size = 0;

        if(hp->max_elem != 0) { //0 Indicates NO Upper limit
            if(hp->max_size == hp->max_elem) {
                fprintf(stderr, "%sHeap size is maxed out, cannot add new nodes", ERROR);
                pthread_mutex_unlock(&(hp->lock_heap));
                return E_REALLOC;
            }

            if(2*hp->max_size > hp->max_elem) {
                new_size = hp->max_elem;
                fprintf(stderr, "%sHeap size is maxed out", WARNING);
            } else {
                new_size = 2*hp->max_size;
            }
        } else {
            new_size = 2*hp->max_size;
        }

        //realloc returns a NULL pointer if it fails, so do NOT make 'hp->array = realloc(...)' or in case of failure 'hp->array' will become NULL
        int* realloc_array = realloc(hp->array, new_size*sizeof(int));
        if(realloc_array == NULL) {
            fprintf(stderr, "%sHeap re-alloc failed", ERROR);
            pthread_mutex_unlock(&(hp->lock_heap));
            return E_REALLOC;
        }

        hp->array = realloc_array; //realloc frees the original memory if the new block is in a different location
        hp->max_size = new_size; //do NOT modify any heap struct attributes until all initializations are EOK like above

        fprintf(stderr, "%sHeap re-allocated to new size [%d], current size [%d]", WARNING, hp->max_size, hp->curr_size);
    }

    hp->array[hp->curr_size++] = value; //Add new node at the end

    //Realloc success, now sift_up
    sift_up(hp, hp->curr_size - 1);

    pthread_mutex_unlock(&(hp->lock_heap));

    return E_EOK;

#undef WARNING
#undef ERROR
}

/****************************************************************************************************************************************************
.
. Peek just returns the first element [MAX or MIN]
.
. Simply checks if the curr_size != 0
.
****************************************************************************************************************************************************/

int
peek(struct heap* hp) {

#define ERROR	"\nError:   (peek) => "

    if(!self_assert(hp)) return E_INIT;

    pthread_mutex_lock(&(hp->lock_heap));

    if(hp->curr_size <= 0) {	//Ideally if the code is perfect, curr_size will NEVER be less than 0
        fprintf(stderr, "%sHeap size is 0, nothing to peek", ERROR);
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_INVAL_PARAM;
    }

    int res = hp->array[0]; //We CANNOT just return 'hp->array[0]' coz it can be a transient value from 'delete' operation, so we need Mutexed extraction

    pthread_mutex_unlock(&(hp->lock_heap));

    //....hp->array[0] CAN change here. It can either be a transient WRONG value or a NEW RIGHT value, BUT our PEEK will NEVER be a transient WRONG value
    //it WILL ONLY be a Stale value that might have been deleted/updated by now

    return res;

#undef ERROR
}

/****************************************************************************************************************************************************
.
. Get Heap Size returns the current size of the heap
.
****************************************************************************************************************************************************/

int
get_heap_size(struct heap* hp) {

    if(!self_assert(hp)) return E_INIT;

    pthread_mutex_lock(&(hp->lock_heap));

    int size = hp->curr_size;

    pthread_mutex_unlock(&(hp->lock_heap));

    return size;
}

/****************************************************************************************************************************************************
.
. CLRS => "Like insertion sort, but unlike merge sort, heapsort sorts in place: only a constant number of array elements are stored outside the input 
           array at any time."
.
. Heapsort runs in O(nlogn)
.
. Assumption: size of dst_array is same/or greater as curr_size in heap
.
****************************************************************************************************************************************************/

int
heap_sort(struct heap* hp, int* dst_array, int size) {

#define ERROR	"\nError:   (heapsort) => "

    if(!self_assert(hp)) return E_INIT;

    if(dst_array == NULL) {
        fprintf(stderr, "%sArray cannot be NULL", ERROR); 
        return E_INVAL_PARAM;
    }

    if(size <= 0) {
        fprintf(stderr, "%sSize of Array [%d] cannot be less than 1", ERROR, size); 
        return E_INVAL_PARAM;
    }

    pthread_mutex_lock(&(hp->lock_heap));

    enum HEAP_TYPE type = hp->type;
    int hp_size = hp->curr_size;

    if(size < hp_size) {
        fprintf(stderr, "%sSize of Array [%d] should be more than size of current heap [%d]", ERROR, size, hp_size); 
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_INVAL_PARAM;
    }

    //create new temporary array and store the heap in it
    int* hp_arr = (int*)calloc(1, hp_size * sizeof(int));
    
    if(hp_arr == NULL) {
        fprintf(stderr, "%sCalloc failed", ERROR); 
        perror("Error");
        pthread_mutex_unlock(&(hp->lock_heap));
        return E_CALLOC;
    }

    memcpy(hp_arr, hp->array, hp_size*sizeof(int));

    //heap has been copied into new temporary memory, now we do NOT need to lock the heap structure...so Unlocking
    pthread_mutex_unlock(&(hp->lock_heap));

    //If it is a Min-Heap, convert to a Max-Heap [ O(n) ]
    if(type == MIN_HEAP) {
        for(int i = hp_size/2; i >=0; i--) {
            heapify(hp_arr, hp_size, i, MAX_HEAP);
        }
    }

    int last  = hp_size - 1;

    for(int i = 0; i < hp_size; i++) {

        //if array goes OOB it can seg-fault
        dst_array[last] = hp_arr[0];
        hp_arr[0] = hp_arr[last];
        heapify(hp_arr, last + 1, 0, MAX_HEAP);
        last--;
    }

    free(hp_arr);

    return E_EOK;

#undef ERROR
}
