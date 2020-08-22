/* This program is to caculate PI using MPI
The algorithm is based on Monte Carlo method. The Monte Carlo method randomly picks up a large number of points in a square. It only counts the ratio of pints in side the circule.

Compile
mpicc 0_MPI_HelloWorld.c -o out

Execute:
mpirun -n 4 ./out
or
mpirun -np 4 out

Hello from process 0 of 4
Hello from process 1 of 4
Hello from process 2 of 4
Hello from process 3 of 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define N 1E8
#define d 1E-8

int main (int argc, char* argv[]) {
  int  error, i;
  double mypi, pi = 0.0,  sum = 0;
  double begin = 0.0, end = 0.0, x, y;
  double PI25DT = 3.141592653589793238462643;
    
  int num_procs = 0;                          /* Number of processes  */
  int my_rank = 0;                            /* My process rank  */

  error = MPI_Init (&argc, &argv);                 /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  
// Synchronize all processes and get the begin time
  MPI_Barrier(MPI_COMM_WORLD);
  
// Returns an elapsed time on the calling processor  
  begin = MPI_Wtime();
  
// Use current time as seed for random generator 
  srand((int)time(0)); 
    
// Each process will caculate a part of the sum
  for (i = my_rank; i < N; i += num_procs) {
        x = rand()/(RAND_MAX + 1.0);
        y = rand()/(RAND_MAX + 1.0);
        if (x*x + y*y < 1.0)
           sum++;
  }
    
// Sum up all results
   MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
// Synchronize all processes and get the end time
   MPI_Barrier(MPI_COMM_WORLD);
 
// Returns an elapsed time on the calling processor    
   end = MPI_Wtime();
    
//Caculate and print PI
   if (my_rank == 0) {
       mypi = 4 * d * pi;
       printf("Time = %fs;    PI = %lf\n", end-begin, mypi);
       printf("PI is approximately %.16f, Error is %.16f\n", mypi, fabs(mypi - PI25DT));
    }
    
    error = MPI_Finalize();               /* Finalize the MPI environment */         
    
    return 0;
}
