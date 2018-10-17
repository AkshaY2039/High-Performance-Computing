# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>

int main ( int argc, char *argv[] );
void timestamp ( );

/******************************************************************************/

int main ( int argc, char *argv[] )

/******************************************************************************/
{
# define N 100

  double array[N];
  int i;
  int master = 0;
  int my_id;
  int numprocs;
  double PI = 3.141592653589793238462643;
  double seed;
  MPI_Status status;
  double sum;
  double sum_all;
/*
  Initialize MPI.
*/
  MPI_Init ( &argc, &argv );
/*
  Get the number of processes.
*/
  MPI_Comm_size ( MPI_COMM_WORLD, &numprocs );
/*
  Determine the rank of this process.
*/
  MPI_Comm_rank ( MPI_COMM_WORLD, &my_id );
/*
  Say hello.
*/
  if ( my_id == master )
  {
    timestamp ( );
    printf ( "\n" );
    printf ( "SUM - Master process:\n" );
    printf ( "  C version\n" );
    printf ( "\n" );
    printf ( "  An MPI example program.\n" );
    printf ( "  The master process computes some coefficients,\n" );
    printf ( "  sends them to each worker process, which sums them.\n" );
    printf ( "\n" );
    printf ( "  Compiled on %s at %s.\n", __DATE__, __TIME__ );
    printf ( "\n" );
    printf ( "  The number of processes available is %d.\n", numprocs );
  }
/*
  The master process initializes the array.
*/
  if ( my_id == master ) 
  {
    seed = 1.2345;

    for ( i = 0; i < N; i++ )
    {
      array[i] = ( double ) i * seed * PI;
    }
  }
/* 
  The master process broadcasts the computed initial values 
  to all the other processes.
*/
  MPI_Bcast ( array, N, MPI_DOUBLE, master, MPI_COMM_WORLD );
/*
  Each process adds up its entries.
*/
  sum = 0.0;
  for ( i = 0; i < N; i++ )
  {
    sum = sum + array[i] * ( double ) my_id;
  }

  printf ( "\n" );
  printf ( "SUM - Process %d:\n", my_id );
  printf ( "  My contribution to the sum is %f\n", sum );
/* 
  Each worker process sends its sum back to the master process.
*/
  if ( my_id != master ) 
  {
    MPI_Send ( &sum, 1, MPI_DOUBLE, master, 1, MPI_COMM_WORLD );
  }
  else 
  {
    sum_all = sum;
    for ( i = 1; i < numprocs; i++ ) 
    {
      MPI_Recv ( &sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, 
        MPI_COMM_WORLD, &status );

      sum_all = sum_all + sum;
    }
  }

  if ( my_id == master )
  {
    printf ( "\n");         
    printf ( "SUM - Master process:\n");         
    printf ( "  The total sum is %.16f\n", sum_all );
  }
/*
  Terminate MPI.
*/
  MPI_Finalize ( );
/*
  Terminate.
*/
  if ( my_id == master )
  {
    printf ( "\n");         
    printf ( "SUM - Master process:\n");         
    printf ( "  Normal end of execution.\n"); 
    printf ( "\n" );
    timestamp ( ); 
  }
  return 0;

# undef N
}
/******************************************************************************/

void timestamp ( )

/******************************************************************************/
/*
  Purpose:

    TIMESTAMP prints the current YMDHMS date as a time stamp.

  Example:

    31 May 2001 09:45:54 AM

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    24 September 2003

  Author:

    John Burkardt

  Parameters:

    None
*/
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  printf ( "%s\n", time_buffer );

  return;
# undef TIME_SIZE
}

