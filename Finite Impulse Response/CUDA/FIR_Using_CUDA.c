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
#include <linux/cuda.h>

#define SIZE				(2048 * 2048)	// Total size of the Arrays
#define THREADS_PER_BLOCK	512				// Number of Threads per block

// kernel code for getting mean filter for the thread
__global__ void mean_filter (int *x, float *y, int n)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	if (index < n) // Avoid accessing beyond the end of the arrays
	{
		if (index == 0) // Exception first element
			y[index] = (x[index] + x[index + 1]) / 3;
		else
			if (index == n - 1) // Exception last element
				y[index] = (x[index - 1] + x[index]) / 3;
			else
				y[index] = (x[index - 1] + x[index] + x[index + 1]) / 3;
	}
}

int main (void)
{
	// host copies of the arrays x, y
	int *host_x;
	float *host_y;
	// device copies of the arrays x, y
	int *dev_x;
	float *dev_y;
	// sizes of both arrays
	int size_x = SIZE * sizeof (int);
	int size_y = SIZE * sizeof (float);

	// Allocate space for device copies of x, y
	cudaMalloc ((void **) &dev_x, size_x);
	cudaMalloc ((void **) &dev_y, size_y);

	// Allocate space for host copies of x, y
	host_x = (int *) malloc (size_x); random_ints (x, SIZE);
	host_y = (float *) malloc (size_y);

	// Copy inputs to device
	cudaMemcpy (dev_x, host_x, size_x, cudaMemcpyHostToDevice);

	// Launch the mean filter kernel code
	mean_filter <<<(SIZE + THREADS_PER_BLOCK -1) / THREADS_PER_BLOCK, THREADS_PER_BLOCK>>> (dev_x, dev_y, SIZE);

	// Copy results back to host
	cudaMemcpy (host_y, dev_y, size_y, cudaMemcpyDeviceToHost);

	// Print input and output
	printf ("|**********INPUT**********||*********OUTPUT**********|\n");
	for (int i = 0; i < SIZE; i++)
	{
		printf ("| x [%6d] = %10.2f || y [%6d] = %10.2f |\n", i, *(host_x + i), i, *(host_y + i));
	}

	// Cleanup
	free (host_x);
	free (host_y);
	cudaFree (dev_x);
	cudaFree (dev_y);

	return 0;
}