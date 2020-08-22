/* This program is to caculate PI using MPI
 x^2 + y^2 = R^2 => y = sqrt(R^2 - x^2), for R=1, -1 < x,y < 1  
 
 But, using a quarter of a  circle,  0 < x,y < 1,  =>  4 * \int_0^1 sqrt(1 - x^2) dx = = pi 
 
The algorithm is based on Trapezium rule. 
If f(x)=4*(1-x^2)^(1/2), then PI is the intergral of f(x) from 0 to 1

$ mpicc 3_MPI_TRAPEZOID_GET_PI.c  -o out -lm
$ mpirun -np 4 ./out
From rank [2]: After Bcast: PI 1963495.408548
From rank [1]: After Bcast: PI 1963495.658451
From rank [3]: After Bcast: PI 1963495.158574
From rank [0]: After Bcast: PI 1963495.908308
np= 4;    Time=0.043254s;    PI=3.141593
PI is approximately 3.1415928535525262, Error is 0.0000001999627330


*/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define N 1E7
#define h 1E-7
#define h2 1E-14   // dx step 

int main (int argc, char* argv[]) {
  int  error, i;
  double PI25DT = 3.141592653589793238462643;
  double mypi = 0.0, begin = 0.0, end = 0.0;
  double pi = 0.0, sum = 0.0, x2;
    
  int num_procs = 0;                               /* Number of processes  */
  int my_rank = 0;                                 /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
    
//Synchronize all processes and get the begin time
  MPI_Barrier(MPI_COMM_WORLD);
    
// Returns an elapsed time on the calling processor 
  begin = MPI_Wtime();
    
//Each process caculates a part of the sum
    for (i = my_rank; i < N; i += num_procs) {
        x2 = h2 * i * i ;
        sum += sqrt(1 - x2);
    }
    
    printf( "From rank [%d]: After Bcast: PI %lf\n", my_rank, sum);
    
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

    error = MPI_Finalize();                  /* Finalize the MPI environment */        
    
    return 0;
}
