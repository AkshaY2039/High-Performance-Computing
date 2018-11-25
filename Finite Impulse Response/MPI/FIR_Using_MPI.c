/*
	High Performance Computing Project
		Finite Impulse Response using MPI
		-	Akshay Kumar (CED15I031)
	The FIR function under consideration is	
		y[n] = (x[n+1] + x[n] + x[n-1]) / 3
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE	100000 // Array Size
#define MASTER		0
#define FROM_MASTER	1
#define FROM_WORKER 2

int main (int argc, char *argv[])
{
	int numtasks, 
		taskid, 
		numworkers, 
		source, 
		dest, 
		mtype, 
		aver_size, 
		extra, 
		offset, 
		i, rc;
	double x[ARRAY_SIZE], 
			y[ARRAY_SIZE];

	srand (time (NULL));
	MPI_Status status;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &taskid);
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);

	if (numtasks < 2)
	{
		printf ("need atleast two mpi tasks, quitting...\n");
		MPI_Abort (MPI_COMM_WORLD, rc);
		exit (1);
	}

	numworkers = numtasks - 1;

	/******************************************* master task *******************************************/
	if (taskid == MASTER)
	{
		printf ("FIR Filter (3 point Average) has started with %d tasks.\n", numtasks);
		printf ("\n\t\t\tInitializing Input Array (x) started...\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			x[i] = rand () % 256;
		printf ("\n\t\t\tInitializing Input Array (x) finished...\n\n");

		// calculating terms for sending data and task distribution
		aver_size = (ARRAY_SIZE - 2) / numworkers;
		extra = (ARRAY_SIZE - 2) % numworkers;
		offset = 0;
		mtype = FROM_MASTER;

		// the first and last element being exceptions are handled by master only
		// also extra work handled by master for Load Balancing
		y[0] = (x[0] + x[1]) / 3;
		y[ARRAY_SIZE - 1] = (x[ARRAY_SIZE - 2] + x[ARRAY_SIZE -1]) / 3;
		for (i = 1; i <= extra; i++)
		{
			y[i] = (x[i-1] + x[i] + x[i+1]) / 3;
		}
		offset = extra + 1;

		// sending matrix data to the worker tasks
		for (dest = 1; dest <= numworkers; dest++)
		{
			printf ("Sending %d terms of 'y' to task %d offset = %d\n", aver_size, dest, offset);
			MPI_Send (&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&aver_size, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&x[offset - 1], aver_size + 2, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset = offset + aver_size;
		}

		// reveive results from worker tasks
		mtype = FROM_WORKER;
		for (source = 1; source <= numworkers; source++)
		{
			MPI_Recv (&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&aver_size, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&y[offset], aver_size, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			printf ("Received results from task %d\n", source);
		}

		// Print input and output
		printf ("|**********INPUT**********||**********OUTPUT**********|\n");
		for (i = 0; i < ARRAY_SIZE; i++)
		{
			printf ("| x [%6d] = %10.2f || y [%6d] = %10.2f |\n", i, x[i], i, y[i]);
		}
		printf ("Done.\n");
	}

	/******************************************* worker task *******************************************/
	if (taskid > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv (&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&aver_size, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&x, aver_size + 2, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

		for (i = 0; i < aver_size; i++)
		{
			y[i + offset] = (x[i-1] + x[i] + x[i+1]) / 3;
		}

		mtype = FROM_WORKER;
		MPI_Send (&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&aver_size, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&y, aver_size, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}

	MPI_Finalize ();
}
