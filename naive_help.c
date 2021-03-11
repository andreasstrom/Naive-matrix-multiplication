/*
File containing functions used by naive matrix multiplication
Author: Andreas Ström
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include <smmintrin.h>

#include "naive_help.h"

static double get_wall_seconds(void){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

void comp_methods(int** a, int** b, int size, int verify){
    int** C = allocate_matrix(size);
    int** D = allocate_matrix(size);
    int** E = allocate_matrix(size);

    printf("Comparing matrix multiplication methods, N = %d\n", size);
    double t = get_wall_seconds();
    naive(C, a, b, size);
    t = get_wall_seconds() - t;
    printf("Naive: %10.10fs\n",t);
    t = get_wall_seconds();
    naive_cacheblocked(D, a, b, size);
    t = get_wall_seconds() - t;
    printf("Cache-Blocked: %10.10fs\n",t);
    t = get_wall_seconds();
    naive_vectorized(E, a, b, size);
    t = get_wall_seconds() - t;
    printf("Vectorized: %10.10fs\n",t);
    
    if(verify){
        printf("\nVerifying results:\n");
        verify_result(C, D, E, size);
    }
    deallocate_matrix(C, size); deallocate_matrix(D, size); deallocate_matrix(E, size);
}

static int** naive(int** dst, int** a, int** b, int size){
    //basic MxM 
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                dst[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return dst;
}

static int** naive_vectorized(int** dst, int** a, int** b, int size) {
    //vectorized func for MxM using SSE4.1
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; j+=4) {
            __m128i res = _mm_setzero_si128();
            for(int k = 0; k < size; k++) {
                __m128i a_vec = _mm_set1_epi32(a[i][k]);
                __m128i b_vec = _mm_loadu_si128((__m128i*)&b[k][j]);
                res = _mm_add_epi32(res, _mm_mullo_epi32(a_vec, b_vec));
            }
            _mm_storeu_si128((__m128i*)&dst[i][j], res);
        }
    }
    return dst;
}

static int** naive_cacheblocked(int** dst, int** a, int** b, int size){
    //cache-blocked MxM
    int i, j, k, bi, bj, bk, bsize = 128;
    
    for(bi=0; bi<size; bi+=bsize)
        for(bj=0; bj<size; bj+=bsize)
            for(bk=0; bk<size; bk+=bsize)
                for(i=0; i<bsize; i++)
                    for(k=0; k<bsize; k++)
                        for(j=0; j<bsize; j++)
                            dst[bi+i][bj+j] += a[bi+i][bk+k]*b[bk+k][bj+j];

    return dst;
}

int** allocate_matrix(int n){
    //func for matrix space allocation
    int** theMatrix = calloc(n , sizeof(int *));
    for(int i = 0; i < n; i++)
        theMatrix[i] = calloc(n , sizeof(int));
    return theMatrix;
}

void deallocate_matrix(int** theMatrix, int n){
    //func to free matrices
    int i;
    for(i = 0; i < n; i++)
        free(theMatrix[i]);
    free(theMatrix);
}

void fill_matrix(int** theMatrix, int n, int min, int max){
    //func to fill matrices with pseudo-random numbers
    int j, i;
    for(j = 0; j < n; j++)
        for(i = 0 ; i < n ; i++)
        theMatrix[i][j] = min + rand() % (max-min);
}


void print_matrix(int** theMatrix, int n){
    //func for printing matrices
    int i, j;
    for(i = 0 ; i < n; i++){
        for(j = 0 ; j < n ; j++){
	        printf("% 4d " , theMatrix[i][j]);
            putchar('\n');
        }
    }
}

int verify_result(int**a, int** b, int** c, int size){
    //func for verifying results from strassen algorithm vs naive algorithm
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        if(a[i][j]!=b[i][j]){printf("First input mismatch\n");return 0;}
        if(a[i][j]!=c[i][j]){printf("Second input mismatch\n");return 0;}
        }
    }
    printf("OK!\n");
    return 1;
}