#include <stdio.h>
#include <time.h>

#include "array_util_public.h"
#include "INTEGER_ARRAY_DATASET.h"
#include "STONKS_DATASET.h"
#include "heap_public.h"

int main(int argc, char** argv) {

//=============================================== ERROR CHECKING ===============================================

 //   int xx = atoi(argv[1]);

//=============================================== PARSE FILES ===============================================

    struct int_array_data_analysis* iada = NULL;
    printf("....%d\n", sizeof(STONKS_DATA)/sizeof(int));
    iada = array_util_init(STONKS_DATA, sizeof(STONKS_DATA)/sizeof(int)); //Once init, object is stored in 'iada' then use functions to work on it
   // array_util_destruct(iada);
    

    //Do Util on Array for Data Analysis
    //int array[9] = {-2,1,-3,4,-1,2,1,-5,4};
    clock_t begin = clock();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime Taken for Max Cluster: %f\n", time_spent);
//    array_util_basic_stats(iada);
//    array_util_sort(iada);
    begin = clock();
    int rc = array_util_basic_stats(iada, true);
    array_util_max_size_cluster(iada, false);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime Taken for Mean Median Mode: %f %d\n", time_spent,rc);
//    array_util_destruct(iada);

    //Print all Data Analysis
    array_util_print_all(iada);

#if 0
//    self_balancing_bst_insert(DATA, DATA_SIZE);

//    int y = 5;
//    int* yy = &y;
//    int** yyy = &yy;

//    printf("\n%p\n%p\n%p\n%p\n", &y, yy, &yy, yyy);

//    struct Point* p;
  //  printf("\nP.x =>%p\n",p);

#define SIZE 17

 //int DATA[SIZE] = {19, 17, 12, 3, 1, 7, 8, 123, 8, 155, 1, 2, 3, 4, 5, 6, 123};

//{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };

 int* arr = NULL;
  heap_init(arr, 2, 4);
 heap_init(DATA, 0, 4);
 printf("\n\n");
 //self_balancing_bst_insert(DATA, SIZE);
 printf("\n\n");
//create_bin_tree(DATA, SIZE);
 printf("\n\nHERE\n\n");

 void* hp = heap_init(DATA, DATA_SIZE, MIN_HEAP);

 int* tmp_arr = (int*)malloc(get_heap_size(hp)*sizeof(int));
 heap_sort(hp, tmp_arr, get_heap_size(hp));


 int tmp = tmp_arr[0];
 for(int i = 1; i < DATA_SIZE; i++) {

    printf(" %d ", tmp);


     if(tmp > tmp_arr[i]) {
          printf("\n\nFAILED\n\n");
          break;
      }

     tmp = tmp_arr[i];

 }

 free(tmp_arr);

 quicksort(DATA, DATA_SIZE);

#endif
 

 

 return 0;

#if 0
 //kth largest
 for(int i = xx; i < SIZE; i++) {
    int top = peek(hp);
    if(top < DATA[i]) {
        delete_node(hp, 0, NULL);
        add_node(hp, DATA[i]); 
    }
 }

 printf("\n\nKth largest = %d\n\n", peek(hp));
  
 self_balancing_bst_insert(hp);

 int val;
 delete_node(hp, -1, &val);
 delete_node(hp, 0, &val);
 self_balancing_bst_insert(hp);

 update_node(hp, 15, -200);
 self_balancing_bst_insert(hp);


 add_node(hp, 666);
 add_node(hp, 667);
 add_node(hp, 668);
 self_balancing_bst_insert(hp);
 printf("\nPEEK %d\n", peek(hp));

 int* tmp_arr = (int*)malloc(get_heap_size(hp)*sizeof(int));
 heap_sort(hp, tmp_arr, get_heap_size(hp) + 10);
// delete_node(DATA,SIZE - 1,15, MAX_HEAP);
// delete_node(DATA,SIZE - 2,5, MAX_HEAP);
// update_node(DATA, SIZE - 2, 0, 125, MAX_HEAP);

  
    
    return 0;

#endif
}
