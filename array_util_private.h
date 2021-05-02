/*
	The actual Object in play by Clients is kept Private. Every Data Processing result is stored in a Compound Structure -
	Convention!! All this is PRIVATE. The Public API has similar structures that the Client can Modify [ but should not ]
	Ideally the Private Member Structures are replicated in Public API by changing Struct name. 

	Because they might want to fill particular struct after processing. But if you change the Client Struct signature it
	cannot be filled because the Server expects both to be the same. So for safety, Server will NEVER accept the Client
	Structure because Client might have changed it, it will ALWAYS return an UNTYPED VOID* bulk data of the Member struct
	and the Client must typecast to dereference the data. If anything crashes it will be the Client not the Server :D
	
	This Object represents one bulk of Data on which we will do analysis. Every Member structure has a Mutex. The idea being 
	that NO 2 THREADS CAN WORK ON SAME ALGO FOR THE SAME OBJECT but they can work on different algos for the same IADA.

*/

#ifndef __INT_ARRAY_UTIL_PRIVATE_H__
#define __INT_ARRAY_UTIL_PRIVATE_H__

#include <stdbool.h> 
#include <pthread.h> 

#include "self_balancing_bst_public.h"
#include "integer_arr_sorting_public.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - IADA STRUCTURES - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

struct int_array_max_cluster {
    int cluster_index_start;
    int cluster_index_end;
    int cluster_size;
};

struct int_array_stats {
    int mean;
    int median;
    int mode;
    int max_num;
    int max_num_index;
    int min_num;
    int min_num_index;
    long long range;
    int* sorted_arr;
};

struct int_array_stonks_stats {
    int num_positive; //Total surges in given period
    int num_negative; //Total plummets in given period
    int max_contig_pos;	//Maximum contiguous surges
    int max_contig_neg;	//Maximum contiguous plummets
    int* arr_contig_pos_dist;	//Distribution of contiguous positives (use in sorted)
    int* arr_contig_pos_cnt;
    int* arr_contig_neg_dist;	//Distribution of contiguous negatives (use in sorted)
    int* arr_contig_neg_cnt;
};

struct int_array_data_analysis {

    struct int_array_data_analysis* obj_id; //Obj Memory Location

    //Data to operate on - Provided by Client! Treated as const data by functions analysing it
    int const* data;
    int size;
    bool init;

    //Find max cluster
    struct int_array_max_cluster* max_cluster;
    bool is_max_cluster_run;
    pthread_mutex_t lock_max_cluster;

    //Basic Statistics
    struct int_array_stats* stats; //Sorts array also coz Median/Mode need it
    bool is_stats_run;
    pthread_mutex_t lock_stats;

    //Advanced Statistics for Stonks
    struct int_array_stonks_stats* stonks_stats;
    bool is_stonks_stats_run;
    pthread_mutex_t lock_stonks_stats;
};

#endif
