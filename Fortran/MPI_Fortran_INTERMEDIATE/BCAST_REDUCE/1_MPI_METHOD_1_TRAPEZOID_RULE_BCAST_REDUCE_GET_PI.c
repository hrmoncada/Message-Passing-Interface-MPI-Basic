/* https://www.alcf.anl.gov/support-center/miracetusvesta/example-program-and-makefile-bgq
 * The method evaluates the integral of 4/(1+x*x) between 0 and 1. 
 * The integral is approximated by a sum of n intervals;
 * The approximation to the integral in each interval is (1/n)*4/(1+x*x).
 
 * File:       
 *    1_MPI_BCAST_GET_PI.c 
 * Purpose:    
 *    A program that uses MPI to calculate PI
 * Compile:    
 *    mpicc -g -Wall -std=C99 -o out 1_MPI_BCAST_GET_PI.c 
 * Usage:        
 *    mpirun -n <number of processes> ./out

Compile :
$ mpicc 1_MPI_BCAST_GET_PI.c  -o out

Execute:
$ mpirun -np 4 ./out
pi is approximately 3.1417259869152536, Error is 0.0001333333254605

*/

#include "mpi.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int n, i;
  double PI25DT = 3.141592653589793238462643;
  double mypi, pi, h, sum, x2;
  double begin = 0.0, end = 0.0;
    
  int num_procs = 0;                              /* Number of processes  */
  int my_rank = 0;                                /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */

  // Returns an elapsed time on the calling processor 
  begin = MPI_Wtime();
  
  /* Number of intervals */
  n = 25;

/* MPI_Bcast - Broadcasts a message from the process with rank "root" to all other processes of the communicator
MPI_Bcast( void *buffer,         // (I/0) starting address of buffer (choice)
           int count,            // (I) number of entries in buffer (integer)
           MPI_Datatype datatype,// (I) data type of buffer (handle)
           int root,             // (I) rank of broadcast root (integer)
           MPI_Comm comm );      // (I) communicator (handle)
  */
  
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  h = 1.0 / (double) n;
  sum = 0.0;

  for ( i = my_rank + 1; i <= n; i += num_procs )  {      
       x2 = h * i * i ;
       sum += sqrt(1 - x2);
  }

/* My PI */ 
  mypi = h * sum;
  printf( "From rank [%d]: After Bcast: PI %f\n", my_rank, mypi);
  
/* MPI_Reduce - Reduces values on all processes to a single value
MPI_Reduce(const void *sendbuf,        // (I) address of send buffer (choice)
                 void *recvbuf,        // (O) address of receive buffer (choice, significant only at root)
                 int count,            // (I) number of elements in send buffer (integer)
                 MPI_Datatype datatype,// (I) data type of elements of send buffer (handle)
                 MPI_Op op,            // (I) reduce operation (handle)
                 int root,             // (I) rank of root process (integer)
                 MPI_Comm comm)        // (I) communicator (handle)
*/
   
  MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

// Returns an elapsed time on the calling processor 
  end = MPI_Wtime();
  
  if (my_rank == 0) {
      printf("Time = %fs;    PI = %lf\n", end-begin, mypi);
      printf("PI is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
  }

  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;
}
