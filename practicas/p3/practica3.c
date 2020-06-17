#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define N 1024

int A[N][N];
int B[N][N];
int C[N][N];

int main() 
{
    int i,j,k;
    double t0, t1;
    int hilos = 8;
	
	
	printf("BUCLE MAS EXTERNO\n");

    t0 = omp_get_wtime();
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            //#pragma omp for schedule(static)
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    t1 = omp_get_wtime();
    printf("[i, j, k] [Hilos: NO] [T: %lf s] (Sin paralelizacion)\n", t1-t0);

	omp_set_num_threads(hilos);
   
    t0 = omp_get_wtime();
    #pragma omp parallel shared(A, B, C)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                //#pragma omp for schedule(static)
                for (k = 0; k < N; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    

    t1 = omp_get_wtime();
    printf("[i, j, k] [Hilos: %d] [T: %lf s]\n", hilos, t1-t0);

    //////////////////////////////////////////////////////////////////////7
    //////////////////////////////////////////////////////////////////////
    omp_set_num_threads(hilos);
    t0 = omp_get_wtime();
    #pragma omp parallel shared(A, B, C)
    {
        #pragma omp for schedule(static)
        for (k = 0; k < N; ++k) {
            for (j = 0; j < N; ++j) {
                //#pragma omp for schedule(static)
                for (i = 0; i < N; ++i) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    

    t1 = omp_get_wtime();
    printf("[k, j, i] [Hilos: %d] [T: %lf s]\n", hilos, t1-t0);


    //////////////////////////////////////////////////////////////////////7
    //////////////////////////////////////////////////////////////////////
    omp_set_num_threads(hilos);
    t0 = omp_get_wtime();
    #pragma omp parallel shared(A, B, C)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < N; ++i) {
            for (k = 0; k < N; ++k) {
                //#pragma omp for schedule(static)
                for (j = 0; j < N; ++j) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    

    t1 = omp_get_wtime();
    printf("[i, k, j] [Hilos: %d] [T: %lf s]\n", hilos, t1-t0);

}