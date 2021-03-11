/*
Header file for for naive matrix multiplication
Author: Andreas Ström
*/

int** allocate_matrix(int n);
void deallocate_matrix(int** theMatrix, int n);
void fill_matrix(int** theMatrix, int n, int min, int max);
void print_matrix(int** theMatrix, int n);

static int** naive(int** dst, int** a, int** b, int size);
static int** naive_cacheblocked(int** dst, int** a, int** b, int size);
static int** naive_vectorized(int** dst, int** a, int** b, int size);
int verify_result(int**a, int** b, int** c, int size);
void comp_methods(int** a, int** b, int size, int verify);
static double get_wall_seconds(void);