#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Variables globales
int num_of_threads; // <--- MODIFICAR NUMERO DE HILOS 4 16 64
int row = 2000,col=2000;

//Variables de tiempo
clock_t t_ini, t_fin;
double secs;

double** A;
double** B;
double** C;


//Funcion paralela
void *matmul(void* rank);
double multiplicacion();

int main(int argc, char* argv[]){

	if (argc < 2) {
        printf("./epractica2.c N\n");
        exit(1);
    }

	printf("-----------------\n");
	printf("[i, j, k]\n");

	num_of_threads = atoi(argv[1]);
	printf("Hilos: %d\n", num_of_threads);
	double suma = 0;

	pthread_t tid[num_of_threads];
	long rank;

	//Reserva de memoria
	A = malloc(row*sizeof(double*)); 
	for(int i=0;i<row;i++)
		A[i]=malloc(col*sizeof(double));

	B = malloc(row*sizeof(double*)); 
	for(int i=0;i<row;i++)
		B[i]=malloc(col*sizeof(double)); 

	C =  malloc(row*sizeof(double*)); 
	for(int i=0;i<row;i++)
		C[i]=malloc(col*sizeof(double));

	
	t_ini = clock();
	//Creación de threads
	for (rank = 0; rank < num_of_threads; rank++)
		pthread_create(&tid[rank], NULL, matmul , (void*) rank);

	//Unión de threads
	for (rank = 0; rank < num_of_threads; rank++)
		pthread_join(tid[rank], NULL);

	//establecemos el tiempo tras la multiplicación
	t_fin = clock();

	//realizamos la resta
	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	
	printf("Total tiempo: %f \n", secs*1000);
	printf("-----------------\n");

	//Fin de proceso padre
	pthread_exit(NULL);
	//Liberación de memoria
	free(A);
	free(B);
	free(C);

	return 0;
}
//-------------------------------------------
// Función que va ejecutarse en cada thread
void *matmul(void* id_arg){
	int i,j,k;
	long  id = (long ) id_arg;
	//Dividimos las operaciones entre los hilos
	int rows_per_thr = col/num_of_threads;
	int start_index = id*rows_per_thr;
	int final_index = (id+1)*rows_per_thr;

	for(i=start_index;i<final_index;i++){
		for(j=0;j<col;j++){
			for(k=0;k<row;k++){
				C[i][j] += A[i][k]*B[k][j]; 
			}
		}
	}
}