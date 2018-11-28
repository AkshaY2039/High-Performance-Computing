/* This code hasn't been tested, just a pseudo code as per my CUDA understanding */
/*
	High Performance Computing Project
		Finite Impulse Response using CUDA
		-	Akshay Kumar (CED15I031)
	The FIR function under consideration is	
		y[n] = (x[n+1] + x[n] + x[n-1]) / 3
*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE				(2048-2048) // Total size of the Arrays
#define THREADS_PER_BLOCK	512			// Number of Threads per block