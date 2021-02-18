//Tree Operations
#include <stdio.h>
#include <math.h>

int
self_balancing_bst_insert(int* data, int size) {

//HEAP
    int base = 0; //Base node
    int level = 0;
    while(base < size) {
       int lvl_cnt = pow(2, level++);
       int l = 0;
       while(l < lvl_cnt) {
           if(base + l >= size) goto exit;
           printf("%d ", data[base + l++]);
       }      
       base = base + lvl_cnt;
       printf("\n");
    }

exit:
   return 0;
}

int main(void) {

 int DATA[16] = { 15, 17, 12, 3, 1, 7, 8, 123, 8, 15, 1, 2, 3, 4, 5, 6 };
 self_balancing_bst_insert(DATA, 16);
return 0;
}


