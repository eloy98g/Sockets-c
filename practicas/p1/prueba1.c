#include <stdio.h>
#include <time.h>

int main()
{
    int A[1024][1024], B[1024][1024], C[1024][1024];
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
        for(int j=0; j<1024; ++j)
            for(int k=0; k<1024; ++k)
                C[i][j]=(C[i][j])+((A[i][k])*(B[k][j]));

    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

    printf("Inicio: %f \n", (double)t_ini);
	printf("Fin: %f \n", (double)t_fin);
	printf("clocks: %f \n", (double)CLOCKS_PER_SEC);

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("[i, j, k]: %.16g milisegundos\n", secs * 1000.0);
    
	return 0;
}