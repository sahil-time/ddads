//Public Util functions to hide Implementation details of Algorithms
//Only satellite strucutres are exposed. The real Object is in array_util_private.h

#ifndef __INT_ARRAY_UTIL_PUBLIC_H__
#define __INT_ARRAY_UTIL_PUBLIC_H__

#include <stdbool.h>

//Error Cases
enum iada_error {
    IADA_E_EOK = 0,
    IADA_E_CACHE = 0,
    IADA_E_INIT = -1,
    IADA_E_CALLOC = -2
};

//For Client use - DO NOT CHANGE THESE STRUCTS - Only used for Typecasting
struct client_int_array_max_cluster {
    int cluster_index_start;
    int cluster_index_end;
    int cluster_size;
};

struct client_int_array_stats {
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

/*
  Creating an Object for the data and adding meta-data that will store the result of data analysis on the data.
  Always init before calling any methods.

  Ideally for a single piece of data, we want to create One IADA Object
*/
struct int_array_data_analysis* array_util_init(int const* array, const int size);

/*
  Destroying the Object
*/
int array_util_destruct(struct int_array_data_analysis* iada);

/*
  Find the max size of cluster in a given array [ also called max sub array problem ]
  Space Complexity = O(n)
  Time Complexity = O(n)
  
   @parameters
>  Provide Object
>  use_cache to return cached value if any
*/
int array_util_max_size_cluster(struct int_array_data_analysis* iada, bool use_cache);

/*
  Calculate mean, median, mode, range [ basic data analysis ]
  For Mean:
    Space Complexity = O(1)
    Time Complexity = O(n)
  For Median / Mode:
    Sort List? Yes! 
    Space Complexity = Sorting
    Time Complexity = Complexity of Sorting + O(n) to find Median and Mode
*/
int array_util_basic_stats(struct int_array_data_analysis* iada, bool use_cache);

/*
  Print all Data Analysis attributes
*/
void array_util_print_all(struct int_array_data_analysis* iada);

#endif
