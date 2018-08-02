#include <stdio.h>
#include <time.h>
#include <omp.h>

int main ()
{
	int rows, columns, i, j, p; //size of matrixes
	rows = columns = 100;

	double a[rows][columns], b[rows][columns], c[rows][columns];
	double start, end;

	for (i = 0; i < rows; i++)
		for (j = 0; j < columns; j++)
		{
			a[i][j] = 999.956;
			b[i][j] = 451654.54545;
		}

	int threads[] = {1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};
	int th_index = sizeof(threads) / sizeof(threads[0]);

	for (p = 0; p < th_index; p++)
	{
		//omp_set_num_threads (threads [p]);
		start = omp_get_wtime (); //addition starts here

		#pragma omp parallel for num_threads(threads[p])
			for (i = 0; i < rows; i++)
			{
				for (j = 0; j < columns; j++)
				{
					c[i][j] = a[i][j] + b[i][j];
				}
			}

		end = omp_get_wtime (); //addition ends here

		printf ("%d\t%lf\n", threads[p], end - start);
	}

	return 0;
}