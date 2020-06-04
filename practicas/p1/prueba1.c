
#include <stdio.h>
#include <time.h>

int main()
{
    printf("holi");
    int A[1024][1024], B[1024][1024], C[1024][1024];
    int count = 1024;
    printf("antes de nada");
	//Variables de tiempo
	clock_t t_ini, t_fin;
	double secs;

    printf("antes de inicializar C");
    // Inicializamos la matriz C.
    for(int i=0; i<count; ++i)
        for(int j=0; j<count; ++j)
            C[i][j] = 0;

    printf("despues de inicializar C");
	//Inicializamos el contador de tiempo al tiempo actual
	t_ini = clock();

    printf("antes de multiplicar");
    // Generamos la matriz C.
    for(int i=0; i<count; ++i)
        for(int j=0; j<count; ++j)
            for(int k=0; k<count; ++k)
                C[i][j] += A[i][k] * B[k][j];

	printf("despues de multiplicar");
    //establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("%.16g milisegundos\n", secs * 1000.0);
	return 0;
}