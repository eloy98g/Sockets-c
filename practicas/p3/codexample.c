# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>

#define MAX 4000
int main ( void );
void timestamp ( void );

int main ( void ){
	double a[MAX][MAX];
	double angle;
	double b[MAX][MAX];
	double c[MAX][MAX];
	int i;
	int j;
	int k;
	int n = MAX;
	double pi = 3.141592653589793;
	double s;
	int thread_num;
	double wtime;

	timestamp ( );

	printf("-----------------\n");
	printf("[i, j, k]\n");

	thread_num = omp_get_max_threads ( );

	printf ( "\n" );
	printf ( "  The number of processors available = %d\n", omp_get_num_procs ( ) );
	printf ( "  The number of threads available    = %d\n", thread_num );
	printf ( "  The matrix order N                 = %d\n", n );
	
	# pragma omp for
	for ( i = 0; i < n; i++ ){
		for ( j = 0; j < n; j++ ){
			c[i][j] = 0.0;
			for ( k = 0; k < n; k++ ){
				c[i][j] +=  a[i][k] * b[k][j];
			}
		}
	}

	}
	wtime = omp_get_wtime ( ) - wtime;
	printf ( "  Elapsed seconds = %g\n", wtime );
	printf ( "  C(100,100)  = %g\n", c[99][99] );
	/*
	Terminate.
	*/
	printf ( "\n" );
	printf ( "MXM_OPENMP:\n" );
	printf ( "  Normal end of execution.\n" );

	printf ( "\n" );
	timestamp ( );

	return 0;
	}
	/******************************************************************************/

	void timestamp ( void ){
	# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm *tm;
	time_t now;

	now = time ( NULL );
	tm = localtime ( &now );

	strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

	printf ( "%s\n", time_buffer );

	return;
	# undef TIME_SIZE
	}