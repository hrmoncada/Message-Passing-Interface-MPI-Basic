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
  MPI_Status status;                               /* Stores the status of whether a communication is successful or not */

  int message = my_rank * 10;
  int count;               /* Status : number of received elements (integer)*/
  int tag = 2017;
  
/*==========================================================================================
AN EXAMPLE TO DEMONSTRATE A POINT-TO-POINT COMMUNICATION BY SENDING AND RECEIVING A NUMBER 
============================================================================================*/
  if (my_rank == 0)        // Rank 0 receives a message from Rank 1
  {
    MPI_Recv(&message, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    
    printf("I am Rank 0. Received message %d from Rank %d\n", message, status.MPI_SOURCE);
    
/* After receiving the message, check the status to determine how many numbers were actually received */
    MPI_Get_count(&status, MPI_INT, &count);

/* Print off the amount of numbers, and also print additional information in the status object */
    printf("Received %d number from rank %d (Message source), with tag = %d\n", count, status.MPI_SOURCE, status.MPI_TAG);
  }
  else if (my_rank == 1)   // Rank 1 sends a message to Rank 0
  {
    MPI_Send(&message, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    printf("I am Rank 1. Sent a message to Rank 0 with %d.\n", tag);
    //printf("0 sent %d numbers to 1\n", number_amount);
  }
  else                     // Other ranks sit and wait
    printf("I am Rank %d. I have nothing to do.\n", my_rank);

  
  MPI_Barrier(MPI_COMM_WORLD);                   /* Synchronize all processes */
  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;
}
