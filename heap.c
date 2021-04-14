/****************************************************************************************************************************************************
. GENERAL INFORMATION:
.
. Whenever you get a problem that talks about the largest or smallest of something, try to think of Heap.
.
*****************************************************************************************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap_private.h"
#include "heap_public.h"

static int self_balancing_bst_insert(int* data, int size) {

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

	HEAPIFY => This can fix a single violation of the Heap property in one iteration. So when you input an un-ordered Array, we go from
'n/2' to 0 because 'n/2 + 1 -> n' are all children which are already Heaps. For each iteration from 'n/2' to 0 we run the Heapify, and we keep
fixing these single violations. 

This is how we build a Heap i.e. Top Down approach because for each iteration we assume that the Left and Right Sub-trees are Heaps and we compare with 
those and let the violation float down to the proper place.

So we can see that Heapify is usefel in building a new Heap from an un-ordered Array. This takesn O(n) time -> Check the proof.

However what about Updates/Deletes on the Heap?

Delete:
	1. Deleting the Child Node -> Straight Forward
	2. Extract Max/Min i.e. Deleting the Root Node -> Replace Root with the Last element, run Heapify and let the Last element float down
	3. Delete Node in the middle -> Same as above

2. and 3. will take 1 Heapify call so O(logn) time.

Update:
	1. Update ANY Node -> Delete that node and then add a new node at the last. 

So Delete will take O(logn) and adding a new node will need one more Heapify so it will also take O(logn). Although Update can be done in a better
way. Just update the Node in question, compare with Leaf and Right sub-trees, if its is fine, then let it bubble up instead of float down. This is
extra complexity to reduce the time.

****************************************************************************************************************************************************/


/****************************************************************************************************************************************************
.
. CLRS => "Like insertion sort, but unlike merge sort, heapsort sorts in place: only a constant number of array elements are stored outside the input 
           array at any time."
.
. Heapsort runs in O(nlogn)
.
****************************************************************************************************************************************************/


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

static void 
heapify(struct heap* hp, int parent_id) {

    int* array = hp->array;
    enum HEAP_TYPE type = hp->type;
    int size = hp->curr_size;

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

        heapify(hp, res_id);
    }
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
        perror("Error");
        return NULL;
    }

    hp->obj_id = hp;

    hp->array = (int*)calloc(1, size*sizeof(int));
    if(hp->array == NULL) {
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
        heapify(hp, i);
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
. Run Heapify so we can do a top-down of the last element - try to fit it somewhere, since the upper half of tree will always be a heap
.
. O(logn) time for this delete operation
.
****************************************************************************************************************************************************/

int 
delete_node(struct heap* hp, int index) {

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

    array[index] = array[hp->curr_size-- - 1];

    //sift-up -> opposite of heapify
    if(((hp->type == MAX_HEAP) && (array[parent_id] < array[index])) || ((hp->type == MIN_HEAP) && (array[parent_id] > array[index]))) {
        sift_up(hp, index);   
    } else {
        heapify(hp, index);
    }

    pthread_mutex_unlock(&(hp->lock_heap));

    return E_EOK;

#undef ERROR
}

/****************************************************************************************************************************************************
.
. Initializing the Heap with 3 inputs. Array, size and type.
.
. We will run Heapify. Init takes O(n) time.
.
. Init works on the original data. It modifies the original array and converts it into a Heap
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
        heapify(hp, index);
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

        //Size already maxed out
        if(hp->max_size == hp->max_elem) {
            fprintf(stderr, "%sHeap size is maxed out, cannot add new nodes", ERROR);
            pthread_mutex_unlock(&(hp->lock_heap));
            return E_REALLOC;
        }

        int new_size = 0;
        if(2*hp->max_size > hp->max_elem) {
            new_size = hp->max_elem;
            fprintf(stderr, "%sHeap size is maxed out", WARNING);
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

    pthread_mutex_unlock(&(hp->lock_heap));

    return hp->array[0];

#undef ERROR
}
