#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define N 4000

int A[N][N];
int B[N][N];
int C[N][N];

int main() 
{
    int i,j,k;
    struct timeval tv1, tv2;
    struct timezone tz;
	double secs; 
    int hilos = 32;
	
	
	printf("BUCLE MAS EXTERNO\n");

	omp_set_num_threads(hilos);
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[i, j, k]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);

	
	/////////////////////////////////////////
	omp_set_num_threads(omp_get_num_procs());
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (j = 0; j < N; ++j) {
        for (k = 0; k < N; ++k) {
            for (i = 0; i < N; ++i) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[j, k, i]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);

	
	//////////////////////////////////////// 
	omp_set_num_threads(omp_get_num_procs());
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; ++i) {
        for (k = 0; k < N; ++k) {
            for (j = 0; j < N; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[i, k, j]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);
	

	/*
	printf("BUCLE MAS INTERNO\n");

	omp_set_num_threads(hilos);
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);
    
	#pragma omp parallel
	for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
			#pragma omp for schedule(dynamic,1)
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[i, j, k]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);

	
	/////////////////////////////////////////
	omp_set_num_threads(omp_get_num_procs());
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);

    #pragma omp parallel
    for (j = 0; j < N; ++j) {
        for (k = 0; k < N; ++k) {
			#pragma omp for schedule(dynamic,1)
            for (i = 0; i < N; ++i) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[j, k, i]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);

	
	//////////////////////////////////////// 
	omp_set_num_threads(omp_get_num_procs());
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    gettimeofday(&tv1, &tz);

    #pragma omp parallel
    for (i = 0; i < N; ++i) {
        for (k = 0; k < N; ++k) {
			#pragma omp for schedule(dynamic,1)
            for (j = 0; j < N; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    secs = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("[i, k, j]: Hilos: %d, T: %.16g milisegundos\n", hilos, secs * 1000.0);*/
}