/* This program is to caculate PI using MPI
The algorithm is based on integral representation of PI.
If f(x)=4/(1+x^2), then PI is the intergral of f(x) from 0 to 1
*/

#include "mpi.h"
#include <math.h>
#include <stdio.h>

#define N 1E7
#define h 1E-7
#define h2 1E-14

int main (int argc, char* argv[]) {
  int error, i;
  double PI25DT = 3.141592653589793238462643;
  double mypi=0.0, sum=0.0, pi=0.0;
  double begin=0.0, end=0.0, x2;
    
  int num_procs = 0;                          /* Number of processes  */
  int my_rank = 0;                            /* My process rank  */

  error = MPI_Init (&argc, &argv);                 /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
    
//Synchronize all processes and get the begin time
    MPI_Barrier(MPI_COMM_WORLD);
    
// Returns an elapsed time on the calling processor  
  begin = MPI_Wtime();
    
//Each process will caculate a part of the sum
    for (i = my_rank; i < N; i += num_procs) {
        x2 = h2 * i * i;
        sum += 1.0/(1.0 + x2);
    }
    
//Sum up all results
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
//Synchronize all processes and get the end time
    MPI_Barrier(MPI_COMM_WORLD);
  
 // Returns an elapsed time on the calling processor     
    end = MPI_Wtime();
    
 //Caculate and print PI
    if (my_rank == 0) {
        mypi = 4 * h * pi;
        printf("Time = %fs;    PI = %lf\n", end-begin, mypi);
        printf("PI is approximately %.16f, Error is %.16f\n", mypi, fabs(mypi - PI25DT));
    }
    
    error = MPI_Finalize();               /* Finalize the MPI environment */   
    
    return 0;
}
