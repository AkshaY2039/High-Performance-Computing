#include "mpi.h"
#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include<string.h>
#define MAXSIZE 10000

int main(int argc, char **argv)
{
	int myid, numprocs;
	int  i, x, low, high;
	float data[MAXSIZE],result,  myresult=9999999999999;
	char fn[255];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if(0 == myid) {
		/* open input file and intialize data */
		strcpy(fn, getenv("PWD"));
		strcat(fn, "/data.txt");
		if( NULL == (fp = fopen(fn, "r")) ) {
			printf("Can't open the input file: %s\n\n", fn);
			exit(1);
		}
		for(i=0; i<MAXSIZE; i++) {
			fscanf(fp, "%f", &data[i]);
		}
	}

	/* broadcast data */
	MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);
	
	/* add portion of data */
	x = MAXSIZE/numprocs;	/* must be an integer */
	low = myid * x;
	high = low + x;
	for(i=low; i<high; i++)
	{
		if(data[i]<myresult)
		{
			myresult= data[i];
		}
	}
	printf("I got %f from %d\n", myresult, myid);

	/* compute global sum */
	MPI_Reduce(&myresult, &result, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);

	if(0 == myid) {
		printf("The min is %f.\n", result);
	}

	MPI_Finalize();
}

	