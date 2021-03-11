/*
Main file for naive matrix multiplication, utilizing cache-blocking or vectorization
Author: Andreas Ström
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include <smmintrin.h>

#include "naive_help.h"



int main(int argc, char *argv[]){

    if(argc<5){
      printf("Input: mat_size[int>0] print[1/0] verify[1/0] min_val[int] max_val[int]\n");
      exit(-1);
    }

    int size = atoi(argv[1]);
    const int print=atoi(argv[2]), verify=atoi(argv[3]), min=atoi(argv[4]), max=atoi(argv[5]);

    if(min>=max || size<0){printf("mat_size < 0  or  min_val >= max_val\n"); exit(-1);}

    const int n = size;
    int** A = allocate_matrix(n); fill_matrix(A, n, min, max);
    int** B = allocate_matrix(n); fill_matrix(B, n, min, max);
    comp_methods(A, B, n, verify);

    deallocate_matrix(A, n); deallocate_matrix(B, n);
    return 0;
}