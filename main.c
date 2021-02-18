#include <stdio.h>
#include <time.h>

#include "array_util_public.h"
#include "INTEGER_ARRAY_DATASET.h"

int
main(void)
{
    struct int_array_data_analysis* iada = NULL;
    iada = array_util_init(DATA, DATA_SIZE); //Once init, object is stored in 'iada' then use functions to work on it
   // array_util_destruct(iada);
    

    //Do Util on Array for Data Analysis
    //int array[9] = {-2,1,-3,4,-1,2,1,-5,4};
    clock_t begin = clock();
    array_util_max_size_cluster(iada, false);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime Taken for Max Cluster: %f\n", time_spent);
//    array_util_basic_stats(iada);
//    array_util_sort(iada);
    begin = clock();
    int rc = array_util_basic_stats(iada, true);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime Taken for Mean Median Mode: %f %d\n", time_spent,rc);

    //Print all Data Analysis
    array_util_print_all(iada);

    self_balancing_bst_insert(DATA, DATA_SIZE);

//    int y = 5;
//    int* yy = &y;
//    int** yyy = &yy;

//    printf("\n%p\n%p\n%p\n%p\n", &y, yy, &yy, yyy);

//    struct Point* p;
  //  printf("\nP.x =>%p\n",p);
    
    return 0;
}
