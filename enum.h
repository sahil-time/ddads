#ifndef __ENUM_H__
#define __ENUM_H__

/****************************************************************************************************************************************************
.
.	SORTING ALGOS
.
****************************************************************************************************************************************************/

enum IADA_SORTING {
#define MULTIPLICATIVE_FACTOR_COMPARISON_SORT 1
//Non-Comparison Sorts [ if MULTIPLICATIVE_FACTOR_COMPARISON_SORT * range <= arr size ]
    IADA_SORTING_COUNTING,
    IADA_SORING_BUCKET,
    IADA_SORTING_RADIX,
//Comparison Sorts
    IADA_SORTING_INSERTION, //Needs shifting of array elements to the right after each iteration - Not Implemented
    IADA_SORTING_SELECTION,     //Not Implemented
    IADA_SORTING_BUBBLE,        //For less than 100 elements use this
    IADA_SORTING_HEAP,  //To play Safe, In-Place but needs a Heap first
    IADA_SORTING_MERGE, //Not In-Place but always O(nlogn) even in worst case
    IADA_SORTING_QUICK  //If In-Place needed
};

/****************************************************************************************************************************************************
.
.	HEAP TYPES
.
****************************************************************************************************************************************************/

enum HEAP_TYPE {
    MIN_HEAP = 0,
    MAX_HEAP
};

#endif
