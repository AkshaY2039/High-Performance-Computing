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

	int n= 100;
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
		#pragma for
		{
			for (int i = 0; i < num_mtx; ++i)
			{
				for (int j = 0; j < num_mtx; ++j)
				{
					#pragma omp flush (MAX)
					{
						if (mtx0[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx0[i][j] > MAX)
									MAX = mtx0[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx1[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx1[i][j] > MAX)
									MAX = mtx1[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx2[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx2[i][j] > MAX)
									MAX = mtx2[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx3[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx3[i][j] > MAX)
								MAX = mtx3[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx4[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx4[i][j] > MAX)
									MAX = mtx4[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx5[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx5[i][j] > MAX)
									MAX = mtx5[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx6[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx6[i][j] > MAX)
									MAX = mtx6[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx7[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx7[i][j] > MAX)
									MAX = mtx7[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx8[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx8[i][j] > MAX)
									MAX = mtx8[i][j];
						}
					}
					#pragma omp flush (MAX)
					{
						if (mtx9[i][j] > MAX)
						{
							#pragma omp critical
								if (mtx9[i][j] > MAX)
									MAX = mtx9[i][j];
						}
					}

				}
			}
		}
	}

	double stop = omp_get_wtime ();
	double time_elapsed = stop - start;
	cout << time_elapsed << endl;

	return 0;
}