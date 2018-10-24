/*
	High Performance Computing Project
		Finite Impulse Response using OpenMP
		-	Akshay Kumar (CED15I031)
	The FIR function under consideration is	
		y[n] = (x[n+1] + x[n] + x[n-1]) / 3
*/

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#define size 100

// main module
int main ()
{
	srand (static_cast <unsigned> (time(0)));
	
	float x[size], y[size];

	// random signal
	for (int i = 0; i < size; i++)
		x[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	double start = omp_get_wtime ();
	// applying filter
	#pragma omp parallel
	{
		y[0] = (x[0] + x[1]) / 3;
		#pragma omp for
		for (int i = 1; i < size-1; i++)
		{
			y[i] = (x[i-1] + x[i] + x[i+1]) / 3;
		}
		y[size - 1] = (x[size - 2] + x[size - 1]) / 3;
	}

	double end = omp_get_wtime ();
	double time_elapsed = end - start;
	cout << time_elapsed << endl;

	return 0;
}
