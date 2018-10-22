#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

int MAX = 0;

int main ()
{
	srand (time (NULL));
	int num_mtx = 10;
	// int no_threads[] = {1,2,4,6,8,10,12,16,20,24,28,32};

	int n = 100;
	int mtx0[n][n], 
		mtx1[n][n], 
		mtx2[n][n], 
		mtx3[n][n], 
		mtx4[n][n], 
		mtx5[n][n], 
		mtx6[n][n], 
		mtx7[n][n], 
		mtx8[n][n], 
		mtx9[n][n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mtx0[i][j] = rand () % 100;
			mtx1[i][j] = rand () % 100;
			mtx2[i][j] = rand () % 100;
			mtx3[i][j] = rand () % 100;
			mtx4[i][j] = rand () % 100;
			mtx5[i][j] = rand () % 100;
			mtx6[i][j] = rand () % 100;
			mtx7[i][j] = rand () % 100;
			mtx8[i][j] = rand () % 100;
			mtx9[i][j] = rand () % 100;
		}
	}   

	double start = omp_get_wtime ();
	#pragma omp parallel shared (MAX)
	{
		int private_max = MAX;
		#pragma for
		{
			for (int i = 0; i < num_mtx; ++i)
			{
				for (int j = 0; j < num_mtx; ++j)
				{
					if (mtx0[i][j] > private_max)
						private_max = mtx0[i][j];
					if (mtx1[i][j] > private_max)
						private_max = mtx1[i][j];
					if (mtx2[i][j] > private_max)
						private_max = mtx2[i][j];
					if (mtx3[i][j] > private_max)
						private_max = mtx3[i][j];
					if (mtx4[i][j] > private_max)
						private_max = mtx4[i][j];
					if (mtx5[i][j] > private_max)
						private_max = mtx5[i][j];
					if (mtx6[i][j] > private_max)
						private_max = mtx6[i][j];
					if (mtx7[i][j] > private_max)
						private_max = mtx7[i][j];
					if (mtx8[i][j] > private_max)
						private_max = mtx8[i][j];
					if (mtx9[i][j] > private_max)
						private_max = mtx9[i][j];
				}
			}
		}
		#pragma omp flush (MAX)
		{
			if (private_max > MAX)
			{
				#pragma omp critical
				if (private_max > MAX)
					MAX = private_max;
			}
		}	 
	}
	double stop = omp_get_wtime ();
	double time_elapsed = stop - start;
	cout << time_elapsed << endl;
	return 0;
}