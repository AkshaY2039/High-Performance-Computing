/**********************************************
 * Median Filter using MPI
**********************************************/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NRA			1024
#define NCA			1024
#define MASTER		0
#define FROM_MASTER	1
#define FROM_WORKER	2

void swap (double a, double b)
{
	double temp = a;
	a = b;
	b = temp;
}

double median (double a[9])
{
	int i, j;
	for (i = 0; i < 5; i++)
		for (j = i+1; j < 9; j++)
			if (a[i] > a[j])
				swap (a[i], a[j]);
	return a[4];
}

int main (int argc, char *argv[])
{
	int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k, l, m, rc;
	double g[NRA][NCA], c_g[NRA][NCA], b[NRA][NCA], c_b[NRA][NCA], r[NRA][NCA], c_r[NRA][NCA], temp_r[9], temp_g[9], temp_b[9];
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
	// master task
	if (taskid == MASTER)
	{
		printf ("MPI Median Filter has started with %d tasks.\n", numtasks);
		printf ("Initializing arrays...\n");
		for (i = 0; i < NRA; i++)
			for (j = 0; j < NCA; j++)
			{
				g[i][j] = rand() % 256;
				b[i][j] = rand() % 256;
				r[i][j] = rand() % 256;
			}

		/* Send matrix data to the worker tasks */
		averow = NRA / numworkers;
		extra = NRA % numworkers;
		offset = 0;
		mtype = FROM_MASTER;
		for (dest = 1; dest <= numworkers; dest++)
		{
			rows = (dest <= extra) ? averow+1 : averow;
			printf ("Sending %d rows to task %d offset = %d\n", rows, dest, offset);
			MPI_Send (&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&g[offset][0], rows * NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&b[offset][0], rows * NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			MPI_Send (&r[offset][0], rows * NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset = offset + rows;
		}

		/* Receive results from worker tasks */
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++)
		{
			source = i;
			MPI_Recv (&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&c_g[offset][0], rows * NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&c_b[offset][0], rows * NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv (&c_r[offset][0], rows * NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			printf ("Received results from task %d\n", source);
		}

		/* Print results */
		printf ("************************GREEN******************************\n");
		printf ("Result Matrix:\n");
		for (i = 0; i < NRA; i++)
		{
			printf ("\n");
			for (j = 0; j < NCA; j++)
			printf ("%6.2f\t", c_g[i][j]);
		}
		printf ("\n******************************************************\n");
		printf ("***************************BLUE***************************\n");
		printf ("Result Matrix:\n");
		for (i = 0; i < NRA; i++)
		{
			printf ("\n");
			for (j = 0; j < NCA; j++)
				printf ("%6.2f\t", c_b[i][j]);
		}
		printf ("\n******************************************************\n");
		printf ("**************************RED****************************\n");
		printf ("Result Matrix:\n");
		for (i = 0; i < NRA; i++)
		{
			printf ("\n");
			for (j = 0; j < NCA; j++)
				printf ("%6.2f\t", c_r[i][j]);
		}
		printf ("\n******************************************************\n");
		printf ("Done.\n");
	}


/**************************** worker task ************************************/
	if (taskid > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv (&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&g, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&b, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv (&r, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

		for (i = 1; i < rows - 1; i++)
			for (j = 1; j < NCA - 1; j++)
			{
				m = 0;
				for (k = i - 1; k <= i + 1; k++)
					for (l = j - 1; l <= j + 1; l++, m++)
					{
						temp_r[m] = r[k][l];
						temp_g[m] = g[k][l];
						temp_b[m] = b[k][l];
					}
				c_r[i][j] = median (temp_r);
				c_g[i][j] = median (temp_g);
				c_b[i][j] = median (temp_b);
			}
		mtype = FROM_WORKER;
		MPI_Send (&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&c_g, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&c_b, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send (&c_r, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize ();
}