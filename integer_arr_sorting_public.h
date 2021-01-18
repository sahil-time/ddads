#ifndef __INTEGER_ARR_SORTING_PUBLIC_H__
#define __INTEGER_ARR_SORTING_PUBLIC_H__

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ENUMS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//Choose Sorting Algo
enum iada_sorting {
#define MULTIPLICATIVE_FACTOR_COMPARISON_SORT 1
//Non-Comparison Sorts [ if MULTIPLICATIVE_FACTOR_COMPARISON_SORT * range <= arr size ]
    IADA_SORTING_COUNTING,
    IADA_SORING_BUCKET,
    IADA_SORTING_RADIX,
//Comparison Sorts
    IADA_SORTING_INSERTION, //Needs shifting of array elements to the right after each iteration - Not Implemented
    IADA_SORTING_SELECTION,	//Not Implemented
    IADA_SORTING_BUBBLE,	//For less than 100 elements use this
    IADA_SORTING_HEAP,	//To play Safe
    IADA_SORTING_MERGE,	//
    IADA_SORTING_QUICK	//If In-Place needed
};

#endif
