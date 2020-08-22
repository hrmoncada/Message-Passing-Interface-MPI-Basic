/* http://hamilton.nuigalway.ie/teaching/AOS/NINE/MPI.htm 

Compile
mpicc 0_MPI_HelloWorld.c -o out

Execute:
mpirun -n 4 ./out  5
or
mpirun -np 4 out  5

Hello from process 0 of 4
Hello from process 1 of 4
Hello from process 2 of 4
Hello from process 3 of 4
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"                /* Include MPI header file that contains the library’s API */
#include <math.h> 

int main (int argc, char* argv[]) { 

  int done = 0, n, i, rc; 
  double PI25DT = 3.141592653589793238462643;
  double mypi, pi, h, sum, x, a;

  int num_procs = 0;                          /* Number of processes  */
  int my_rank = 0;                            /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */

  while (!done){ 
     if (my_rank == 0) { 
        printf("Enter the number of intervals: (0 quits) "); 
        scanf("%d",&n); 
     }
     
     MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     
     if (n == 0) break; 

     h = 1.0 / (double) n; 
     sum = 0.0; 
     for (i = my_rank + 1; i <= n; i += num_procs) { 
        x = h * ((double)i - 0.5); 
        sum += 4.0 / (1.0 + x*x); 
     } 
    
     mypi = h * sum;
     MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } 
  if (my_rank == 0) 
     printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT)); 

  MPI_Finalize();                 /* Finalize the MPI environment */  
    
  return 0;
}
