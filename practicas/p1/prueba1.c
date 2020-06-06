#include <stdio.h>
#include <time.h>

int main()
{
    int A[1024][1024], B[1024][1040], C[1024][1024];
	//Variables de tiempo
	clock_t t_ini, t_fin;
	double secs;
    
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1040; ++j)
            for(int k=0; k<1024; ++k)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[i, j, k]: %.16g milisegundos\n", secs * 1000.0);
    
	return 0;
}














/*
///////////////////////////////////////////
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int k=0; k<1024; ++k)
            for(int j=0; j<1040; ++j)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[i, k, j]: %.16g milisegundos\n", secs * 1000.0);

    ///////////////////////////////////////////
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int k=0; k<1024; ++k)
        for(int i=0; i<1024; ++i)
            for(int j=0; j<1040; ++j)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[k, i, j]: %.16g milisegundos\n", secs * 1000.0);

    ///////////////////////////////////////////
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int k=0; k<1024; ++k)
        for(int j=0; j<1040; ++j)
            for(int i=0; i<1024; ++i)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[k, j, i]: %.16g milisegundos\n", secs * 1000.0);

    ///////////////////////////////////////////
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int j=0; j<1040; ++j)
        for(int i=0; i<1024; ++i)
            for(int k=0; k<1024; ++k)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[j, i, k]: %.16g milisegundos\n", secs * 1000.0);

    ///////////////////////////////////////////
    // Inicializamos la matriz C.
    for(int i=0; i<1024; ++i)
        for(int j=0; j<1024; ++j)
            C[i][j] = 0;

	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    // Generamos la matriz C.
    for(int j=0; j<1040; ++j)
        for(int k=0; k<1024; ++k)
            for(int i=0; i<1024; ++i)
                C[i][j] += A[i][k] * B[k][j];

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[j, k, i]: %.16g milisegundos\n", secs * 1000.0);

*/