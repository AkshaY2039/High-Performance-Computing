#include "mpi.h" 
#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>
int main(int argc, char *argv[]) {    
	 int myid, numprocs;     
	double startwtime, 
	endwtime;     
	int  namelen;  
	int ARRAY_SIZE = 100000;   
	float vector1[ARRAY_SIZE];    
	float vector2[ARRAY_SIZE];
	float vector3[ARRAY_SIZE];
	 int i, j;
	 int s, s0;     
	double totalTime; 
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc,&argv);     
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);     	MPI_Comm_rank(MPI_COMM_WORLD,&myid);     	MPI_Get_processor_name(processor_name,&namelen); 
	fprintf(stderr,"Process %d on %s\n", myid, processor_name);     
	fflush(stderr); 
 // Vector 1 Reading
    for (i=0; i < ARRAY_SIZE; i++)          
	vector1[i] = i; 
 // Vector 1 Reading
    for (i=0; i < ARRAY_SIZE; i++)          
	vector2[i] = i; 

if(myid == 0)
{
	MPI_Bcast(&ARRAY_SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD); 
	s = (int) floor(ARRAY_SIZE/numprocs);
	s0 = ARRAY_SIZE%numprocs; 
	float vector1Receive[s];    
	float vector2Receive[s];
	float vector3Receive[s];
	if (s0 != 0)
	{       
		s = s + 1;
		for(i=0; i < ((s * numprocs) - ARRAY_SIZE); i++)
		{
			vector1[ARRAY_SIZE + i] = 0;
			vector2[ARRAY_SIZE + i] = 0;
		}
	}
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(vector1, s, MPI_INT, vector1Receive, s, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(vector2, s, MPI_INT, vector2Receive, s, MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0; i<s; i++)
	{
		vector3Receive[i] = vector1Receive[i] * vector2Receive[i];
	}	
	MPI_Gather(vector3Receive, s, MPI_INT, vector3, s, MPI_INT, 0, MPI_COMM_WORLD);
	 
	double sum = 0;
	for(i=0; i<ARRAY_SIZE; i++)
	        sum+=vector3[i];
        
        printf("%f\n", vector3[i]);
}
else
{	
	MPI_Bcast(&ARRAY_SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	float vector1Receive[s];    
	float vector2Receive[s];
	float vector3Receive[s];
	MPI_Scatter(vector1, s, MPI_INT, vector1Receive, s, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(vector2, s, MPI_INT, vector2Receive, s, MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0; i<s; i++)
	{
		vector3Receive[i] = vector1Receive[i] * vector2Receive[i];
	}	
	MPI_Gather(vector3Receive, s, MPI_INT, vector3, s, MPI_INT, 0, MPI_COMM_WORLD);

}
MPI_Finalize();
}
