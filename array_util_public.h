//Public Util functions to hide Implementation details of Algorithms
//To expose structures define them, to keep them private define in .c of implementation file and just use Forward Declaration here

#ifndef __INT_ARRAY_UTIL_PUBLIC_H__
#define __INT_ARRAY_UTIL_PUBLIC_H__

#include <stdbool.h>

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
    int range;
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
  
    //Sort the array
    int* sorted_arr; //Pointer to sorted Array
    bool is_sorted_arr_run;

    //Basic Statistic
    struct int_array_stats* stats;
    bool is_stats_run;
};

/*
  Creating an Object for the data and adding meta-data that will store the result of data analysis on the data.
  Always init before calling any methods.

  Ideally for a single piece of data, we want to create One IADA Object
*/
struct int_array_data_analysis* array_util_init(int const* array, const int size);

/*
  Find the max size of cluster in a given array [ also called max sub array problem ]
  Space Complexity = O(n)
  Time Complexity = O(n)
  
   @parameters
>  Pointer to Array (not **), size of array, and struct to fill values
>  use_cache to return cached value if any
*/
void array_util_max_size_cluster(struct int_array_data_analysis* iada, bool use_cache);

/*
  Calculate mean, median, mode, range [ basic data analysis ]
  For Mean:
    Space Complexity = O(1)
    Time Complexity = O(n)
  For Median:
    Sort List?
    Space Complexity = O(n)

*/
void array_util_basic_stats(struct int_array_data_analysis* iada, bool use_cache);

/*
  Print all Data Analysis attributes
*/
void array_util_print_all(struct int_array_data_analysis* iada);

#endif
