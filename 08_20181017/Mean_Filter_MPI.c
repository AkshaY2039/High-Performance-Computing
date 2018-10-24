/**********************************************
**********************************************/
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

#define NRA 200
#define NCA 200
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main ()
{
	int numtasks,taskid,numworkers,source,dest,mtype,rows,averow,extra,offset,i,j,k,l,rc;
	double g[NRA][NCA],c_g[NRA][NCA],sum_g,b[NRA][NCA],c_b[NRA][NCA],sum_b,r[NRA][NCA],c_r[NRA][NCA],sum_r;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	if(numtasks<2)
	{
		printf("need atleast two mpi tasks, quitting...\n");
		MPI_Abort(MPI_COMM_WORLD,rc);
		exit(1);
	}
	numworkers=numtasks-1;
	//master task
	if (taskid == MASTER)
   {
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      printf("Initializing arrays...\n");
      for (i=0; i<NRA; i++)
         for (j=0; j<NCA; j++){
            g[i][j]=1;
	    b[i][j]=1;
	    r[i][j]=1;
        	c_g[i][j]=1;
		c_b[i][j]=1;
		c_r[i][j]=1;
        }

      /* Send matrix data to the worker tasks */
      averow = NRA/numworkers;
      extra = NRA%numworkers;
      offset = 0;
      mtype = FROM_MASTER;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&g[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,MPI_COMM_WORLD);
	MPI_Send(&b[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,MPI_COMM_WORLD);
	MPI_Send(&r[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,MPI_COMM_WORLD);
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c_g[offset][0], rows*NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
	MPI_Recv(&c_b[offset][0], rows*NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
	MPI_Recv(&c_r[offset][0], rows*NCA, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

      /* Print results */
      printf("************************GREEN******************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCA; j++) 
            printf("%6.2f   ", c_g[i][j]);
      }
      printf("\n******************************************************\n");
 printf("***************************BLUE***************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCA; j++) 
            printf("%6.2f   ", c_b[i][j]);
      }
      printf("\n******************************************************\n");
 printf("**************************RED****************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCA; j++) 
            printf("%6.2f   ", c_r[i][j]);
      }
      printf("\n******************************************************\n");
      printf ("Done.\n");
   }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&g, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&r, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

      for (i=0; i<rows; i++)
         for (j=0; j<NCA; j++)
         {
         	sum_g=0;
		sum_b=0;
		sum_r=0;
         	for(k=i;k<=i+2;k++)
         		for (l=j;l<=j+2;l++)
			{
         			sum_g+=g[k][l];
				sum_b+=b[k][l];
				sum_r+=r[k][l];
			}
         		c_g[i+1][j+1]=sum_g/9;
			c_b[i+1][j+1]=sum_b/9;
			c_r[i+1][j+1]=sum_r/9;
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c_g, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c_b, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c_r, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}
}