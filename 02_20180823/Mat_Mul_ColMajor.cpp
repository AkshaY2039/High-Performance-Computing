#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main ()
{
	int l = 100;
	int a[l][l], b[l][l], c[l][l], m = l, n = l, p = l, q = l, i, j, k;
	// int no_threads[] = {1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};

	for (i = 0; i < m; ++i)
		for (j = 0; j < n; ++j)
			a[i][j] = 5;

	for (i = 0; i < p; ++i)
		for (j = 0; j < q; ++j)
			b[i][j] = 7;

	double start = omp_get_wtime ();
	#pragma omp parallel
	{
		#pragma omp for
		for (i = 0; i < m; ++i)
		{
			for (j = 0; j < q; ++j)
			{
				c[j][i] = 0;
				for (k = 0; k < n; ++k)
					c[j][i] = c[j][i] + (a[j][k] * b[k][i]);
			}
		}
	}
	double end = omp_get_wtime ();
	double time_elapsed = end - start;
	cout << time_elapsed << endl;
	return 0;
}