#include <stdio.h>
#include <time.h>
#include <omp.h>

#define size 100

int main ()
{
	int i, j, p;
	
	double a[size][size] = {999.956};
	double b[size][size] = {4516.524};
	double c[size][size];
	double start, end;

	int threads[] = {1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};
	int th_index = sizeof(threads) / sizeof(threads[0]);

	for (p = 0; p < th_index; p++)
	{
		//omp_set_num_threads (threads [p]);
		start = omp_get_wtime (); //addition starts here

		#pragma omp parallel for num_threads(threads[p]) shared (a, b) private (i, j)
			for (i = 0; i < size; i++)
			{
				for (j = 0; j < size; j++)
				{
					c[i][j] = a[i][j] + b[i][j];
				}
			}

		end = omp_get_wtime (); //addition ends here

		printf ("%d\t%lf\n", threads[p], end - start);
	}

	return 0;
}