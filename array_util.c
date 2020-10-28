#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_util_private.h"
#include "array_util_public.h" //Just to check Function Signatures given to Client against the Implementation Signatures

/*
	FORMAT TO IMPLEMENT ANY PUBLIC API EXCEPT INIT =>

	return_type (almost always void => No side effect programming)
	function_name(function_params...) {

		> if(!self_assert(iada)) return;

		> pthread_mutex_lock
              
		> check if use_cache is True
 
		> Free Malloc memory before allocating again if any

		> ptr = calloc(required structures)
		> if(ptr == NULL)...

		...
		... Add code here
		...

		> iada->is_max_cluster_run = true;

		> pthread_mutex_unlock
	}

	SUGGESTIONS TO CREATE A PUBLIC API =>
	1. Think about what happens if functions are called multiple times un-intentionally.
	   > If you are callocing memory and not freeing.
	   > Are they re-entrant [ do NOT work with per Thread data or Locks ]
	   > Are they thread safe [ do NOT work on Global data, just on local data ]
	   > Identify Critical regions where you need Locking [ Avoid if possible ]

*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  INIT/DESTRUCT FUNCTION - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static bool
self_assert(struct int_array_data_analysis* iada) {

    if(iada == NULL) {
        fprintf(stderr, "\nError: Struct Pointer cannot be NULL");
        return false;
    }

    if(!iada->init) {
        fprintf(stderr, "\nError: Init not done!");
        return false;
    }

   return true;
}

//Every INIT will create a New Object [ its like 'new Object()' in Java ]
struct int_array_data_analysis*
array_util_init(int const* data, const int size) {

    if(data == NULL) {
        fprintf(stderr, "\nError: Array Data cannot be NULL");
        return NULL;
    }

    if(size < 1) {
        fprintf(stderr, "\nError: Array Size not valid");
        return NULL;
    }

    struct int_array_data_analysis* iada = (struct int_array_data_analysis*)calloc(1, sizeof(struct int_array_data_analysis)); //Init Structure Object
    if(iada == NULL) {
        perror("Error");
        return NULL;
    }

    iada->obj_id = iada;

    iada->data = data;
    iada->size = size;

    //Init Mutexes
    pthread_mutex_init(&iada->lock_max_cluster, NULL);
    pthread_mutex_init(&iada->lock_sorted_arr, NULL);
    pthread_mutex_init(&iada->lock_stats, NULL);

    iada->init = true;

    return iada;
}

//Init-Destruct are NOT Synced. Can cause Producer-Consumer Problem [ Check in UT ]
void
array_util_destruct(struct int_array_data_analysis* iada) {

    //Check if Malloc has been done ( We do NOT want to free random memory that might not be on Heap )
    //Best is to check if iada->init  == true

    if(iada == NULL) {
        fprintf(stderr, "\nError: Object cannot be NULL");
        return;
    }

    if(!iada->init) { //Can Seg-fault if iada is NOT-NULL but pointing to random memory as de-reference will fail
        fprintf(stderr, "\nError: Object not initialized");
        return;
    }

    free(iada);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - MAX CLUSTER FUNCTIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - */
static void
array_util_print_max_cluster(struct int_array_max_cluster* clust) {

    if(clust == NULL) {
        fprintf(stderr, "\nError: Clust Pointer cannot be NULL");
        return;
    }

    printf("\n");
    printf("\n\tCluster Index Start: %d \
           \n\tCluster Index End: %d \
           \n\tCluster Size: %d", 
           clust->cluster_index_start,
           clust->cluster_index_end,
           clust->cluster_size);
}

void
array_util_max_size_cluster(struct int_array_data_analysis* iada, bool use_cache) {
   
    if(!self_assert(iada)) return;

    pthread_mutex_lock(&iada->lock_max_cluster);
 
    //Check if use_cache = True
    if(use_cache) {
        if(iada->is_max_cluster_run) {
            return;
        }
    }
    
    //Before allocating free previous calloc [ or else leak ]
    if(iada->max_cluster){
        free(iada->max_cluster);
    }

    //Init Max Cluster [ Before you calloc again make sure you free it ]
    iada->max_cluster = (struct int_array_max_cluster*)calloc(1, sizeof(struct int_array_max_cluster));
    if(iada->max_cluster == NULL) {
        perror("Error");
    }

    //New variables to avoid name cluttering in algo
    int* clust_size = &(iada->max_cluster->cluster_size);
    int* clust_start = &(iada->max_cluster->cluster_index_start);
    int* clust_end = &(iada->max_cluster->cluster_index_end);
    int const* array = iada->data;	//Point to actual data
    int size = iada->size;	//Create new Array because we DONT want to change existing Array [ also we have additional compile check - int const* ]
    int res[size];    //Using VLA -> If input is large, consider calloc because Stack overflow.
    res[0] = array[0];
    
    /* - - - - - - - - - - Kadane's algo Start - - - - - - - - - -*/
    *clust_size = array[0];
    *clust_end = 0;

    for(int i = 1; i < size; i++) {
        if(array[i] +  res[i - 1] > array[i]) {
            res[i] = array[i] + res[i - 1];
        } else {
            res[i] = array[i]; //Start of new Sub-Array
        }

        if(*clust_size < res[i]) { 
            *clust_size = res[i];
            *clust_end = i;
        }
    }
    /* - - - - - - - - - - Kadane's algo End - - - - - - - - - -*/

    //Find start index by backtracking
    int sum = *clust_size;
    int index = *clust_end;
    while(sum) {
        sum = sum - array[index--];
    }

    *clust_start = index + 1; //off by 1
    
    iada->is_max_cluster_run = true;

    pthread_mutex_unlock(&iada->lock_max_cluster);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - BASIC STATS RUN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static void
array_util_print_basic_stats(struct int_array_stats* stats) {

    if(stats == NULL) {
        fprintf(stderr, "\nError: Stats Pointer cannot be NULL");
        return;
    }

    printf("\n");
    printf("\n\tArray Stats Mean: %d \
           \n\tArray Stats Max Number: %d | Index: %d \
           \n\tArray Stats Min Number: %d | Index: %d \
           \n\tArray Stats Range: %lld", 
           stats->mean,
           stats->max_num, stats->max_num_index,
           stats->min_num, stats->min_num_index,
           stats->range);
}

static void
array_util_basic_stats_calc_mean(struct int_array_data_analysis* iada) {

    int const* data = iada->data;
    long long sum = data[0];
    int min = data[0];
    int max = data[0];
    int min_index = 0;
    int max_index = 0;

    for(int i = 1; i < iada->size; i++) {
        sum = sum + data[i];

        //Check min
        if(min > data[i]) {
            min = data[i];
            min_index = i;
        }

        //Check max
        if(max < data[i]) {
            max = data[i];
            max_index = i;
        }
    }

    iada->stats->mean = sum/iada->size;
    iada->stats->max_num = max;
    iada->stats->max_num_index = max_index;
    iada->stats->min_num = min;
    iada->stats->min_num_index = min_index;
    iada->stats->range = (long long)max - (long long)min;
}

static void
array_util_basic_stats_calc_median_mode(struct int_array_data_analysis* iada) {

    //Check if Sorted or else run it


}

void
array_util_basic_stats(struct int_array_data_analysis* iada, bool use_cache) {

    if(!self_assert(iada)) return;

    pthread_mutex_lock(&iada->lock_stats);

    //Check if use_cache = True
    if(use_cache) {
        if(iada->is_stats_run) {
            return;
        }
    }
    
    //Before allocating free previous calloc [ or else leak ] - But quick calls on this function on same IADA will SegFault
    if(iada->stats){
        free(iada->stats);
    }

    //Init Array Stats
    iada->stats = (struct int_array_stats*)calloc(1, sizeof(struct int_array_stats));
    if(iada->stats == NULL) {
        perror("Error");
    }

    //Calculate Mean, min and max
    array_util_basic_stats_calc_mean(iada);

    /* - - - - - - Median needs Sorting, if Max_Num - Min_Num is relatively small try using
       - - - - - - Non-Comparison Sort to get better Time & Space complexity.
       - - - - - - Mode can use the same provided by Median. */

    //Calculate Median & Mode
    array_util_basic_stats_calc_median_mode(iada);

    iada->is_stats_run = true;

    pthread_mutex_unlock(&iada->lock_stats);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - PRINT ALL ATTRIBUTES - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
array_util_print_all(struct int_array_data_analysis* iada) {

    if(!self_assert(iada)) return;

    printf("\n| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -|\n");
    printf("\nFor Obj ID: %p", iada->obj_id);
    printf("\nBase Obj Size: %lu bytes", sizeof(struct int_array_data_analysis));

    if(iada->is_max_cluster_run) {
        array_util_print_max_cluster(iada->max_cluster);
    }

    if(iada->is_sorted_arr_run) {

    }

    if(iada->is_stats_run) {
        array_util_print_basic_stats(iada->stats);
    }

    printf("\n\n| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -|\n");
}

