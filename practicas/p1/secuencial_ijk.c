#include <stdio.h>
#include <time.h>

int main(){
	int count = 1024;
	int A[1024][1024], B[1024][1024], C[1024][1024];
	clock_t t_ini, t_fin;
	double secs;


	t_ini = clock();
	for (int i = 0; i < count; ++i)
	{
		for (int k = 0; k < count; ++k)
		{
			for (int j = 0; j < count; ++j)
			{
				C[i][j]+=A[i][k] * B[k][j];
			}
		}
	}
	t_fin = clock();

	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("%.16g milisegundos\n", secs * 1000.0);
	return 0;
}