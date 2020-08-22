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
#define  MASTER 0               /* my_rank 0 is the MASTER */

int main (int argc, char *argv[]) {
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */
  int   len;                                       /* Hostname lenth */   
  char hostname[MPI_MAX_PROCESSOR_NAME];           /* Processor name, This must be an array of size at least MPI_MAX_PROCESSOR_NAME. */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  
  MPI_Get_processor_name(hostname, &len);          /* Gets the name of the processor*/

  printf ("Hello from task %d on %s with hostname size %d !!\n", my_rank, hostname, len);
  
  if (my_rank == MASTER)
     printf("MASTER %d : Number of MPI tasks is: %d\n",my_rank, num_procs);
  
  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;

}

