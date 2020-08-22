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
#include <string.h>  /* For strlen             */
#include "mpi.h"                /* Include MPI header file that contains the library’s API */
const int MAX_STRING = 100;

int main (int argc, char* argv[]) {
  char  greeting[MAX_STRING];                      /* String storing message,  char buffer */  
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;                               /* Stores the status of whether a communication is successful or not */
  
/*==========================================================================================
AN EXAMPLE TO DEMONSTRATE A POINT-TO-POINT COMMUNICATION BY SENDING AND RECEIVING A STRING 
============================================================================================*/

  int count;               /* Status : number of received elements (integer)*/
  int tag = 2017;
  
  if (my_rank == 0)        // Rank 0 receives a message from Rank 1
  {
    MPI_Recv(greeting, MAX_STRING, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);

/* Print reveive message from process rank q */
   printf("I am Rank 0. Received message from Rank %d\n", status.MPI_SOURCE);
/* Print reveive message from process rank q */
   printf("%s\n", greeting);          
/* After receiving the message, check the status to determine how many numbers were actually received */
   MPI_Get_count(&status, MPI_INT, &count);

/* Print off the amount of numbers, and also print additional information in the status object */
    printf("Received %d numbers from rank %d (Message source), with tag = %d\n", count, status.MPI_SOURCE, status.MPI_TAG);
  }
  else if (my_rank == 1)   // Rank 1 sends a message to Rank 0
  { /* Create message, sprintf stands for “String print”. Instead of printing on console, it store output on char buffer which are specified in sprintf */
    sprintf(greeting, "SEND : Greetings from process %d of %d!!!", my_rank, num_procs);
/* Send message to process 0 */       
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    printf("I am Rank 1. Sent a message to Rank 0 with %d.\n", tag);
    //printf("0 sent %d numbers to 1\n", number_amount);
  }
  else                     // Other ranks sit and wait
    printf("I am Rank %d. I have nothing to do.\n", my_rank);

  
  MPI_Barrier(MPI_COMM_WORLD);                   /* Synchronize all processes */
  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;
}