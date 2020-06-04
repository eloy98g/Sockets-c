#include <stdio.h>
#include <time.h>

int main(){
	int count = 2048;
	int A[count][count], B[count][count], C[count][count];
	clock_t t_ini, t_fin;
	double secs;

	t_ini = clock();
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < count; ++j)
		{
			for (int k = 0; k < count; ++k)
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