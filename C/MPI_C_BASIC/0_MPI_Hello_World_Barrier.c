/*
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
#include "mpi.h"                /* Include MPI header file that contains the library’s API */

int main (int argc, char* argv[]) {
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */

  printf( "Hello from process %d of %d\n", my_rank, num_procs );

  MPI_Barrier(MPI_COMM_WORLD);                     /* Wait for all processes, or Synchronize all processes */
  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;
}
