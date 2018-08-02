#include <stdio.h>
#include <time.h>
#include <omp.h>

#define size 100

int main ()
{
	int a[size], b[size], i, p;
	double start, end;

	int threads[] = {1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};
	int th_index = sizeof(threads) / sizeof(threads[0]);

	for (p = 0; p < th_index; p++)
	{
		//omp_set_num_threads (threads [p]);
		start = omp_get_wtime (); //array operation starts here

		#pragma omp parallel for num_threads(threads[p])
			for (i = 0; i < size; i++)
			{
				a[i] = (i+1)*1.0;
				b[i] = (i+1)*2.0;
			}

		end = omp_get_wtime (); //array operation ends here

		printf ("%d\t%lf\n", threads[p], end - start);
	}

	return 0;
}