#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 2000
int num_of_threads = 64;
double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

void *matmul(void* rank);

int main()
{
    
	
	clock_t t_ini, t_fin;
	double secs;
    
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	t_ini = clock();

     for(int i=0; i<1024; ++i)
        for(int j=0; j<1040; ++j)
            for(int k=0; k<1024; ++k)
                C[i][j] += A[i][k] * B[k][j];

   	t_fin = clock();

	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[i, j, k]: %.16g milisegundos\n", secs * 1000.0);
    
	return 0;
}