#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define TRIALS      20
#define ARRAY_SIZE  1000000
int main(int argc, char *argv[])
{
    int myid, numprocs;
    double startwtime, endwtime;
    int  namelen;
    int numbers[ARRAY_SIZE];
    int i, j, sum, part_sum;
    int s, s0, startIndex, endIndex;
    double totalTime;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    fprintf(stderr,"Process %d on %s\n", myid, processor_name);
    fflush(stderr);
    for (i=0; i < ARRAY_SIZE; i++)
        numbers[i] = 1;
    if (myid == 0)
    {
       s = (int) floor(ARRAY_SIZE/numprocs);
       s0 = s + ARRAY_SIZE%numprocs;
       //printf("s=%d , s0= %d\n", s, s0);
    }
    MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&s0, 1, MPI_INT, 0, MPI_COMM_WORLD);
    startIndex = s0 + (myid - 1)*s;
    endIndex = startIndex + s;
    totalTime = 0;
        if (myid == 0)
        {
            startwtime = MPI_Wtime();
        }
        sum = 0;
        part_sum = 0;
        if (myid == 0)  // master
        {
            // compute sum of master's numbers
            for (i = 0; i < s0; i++)
            {
                part_sum += numbers[i];
            }
	    //printf("I got %d from %d\n",part_sum, myid);
        } else 
        {
            for (i = startIndex; i < endIndex; i++)
            {
                part_sum += numbers[i];
            }
	    //printf("I got %d from %d\n",part_sum, myid);
        }
        MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (myid == 0)
       printf("Sum = %d \n", sum);
    MPI_Finalize();
}
