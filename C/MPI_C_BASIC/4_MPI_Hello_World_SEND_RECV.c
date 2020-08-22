/******************************************************************************
* FILE: mpi_helloBsend.c
* DESCRIPTION:
*   MPI tutorial example code: Simple hello world program that uses blocking
*   send/receive routines.
* AUTHOR: Blaise Barney
* LAST REVISED: 06/08/15
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* For strlen             */
#include "mpi.h"     /* Include MPI header file that contains the library’s API */

const int MAX_STRING = 100;

#define  MASTER 0

int main (int argc, char *argv[]) {
  int   num_procs;              /* Number of processes  */
  int   my_rank;                /* My process rank  */
  char  greeting[MAX_STRING];   /* String storing message,  char buffer */ 
  char  messages[MAX_STRING];   /* String storing message,  char buffer */

  int len;                      /* Length (in characters) of the hostname */
  int partner;
  char hostname[MPI_MAX_PROCESSOR_NAME];

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;                               /* Stores the status of whether a communication is successful or not */
  
/*==========================================================================================
AN EXAMPLE TO DEMONSTRATE A POINT-TO-POINT COMMUNICATION BY SENDING AND RECEIVING A STRING 
============================================================================================*/ 
  int tag = 2017;
  int count;               /* Status : number of received elements (integer)*/
 
/* need an even number of tasks  */
if (num_procs % 2 != 0) {
   if (my_rank == MASTER) 
      printf("Quitting. Need an even number of tasks: num_procs=%d\n", num_procs);
   
} else {
   if (my_rank == MASTER) {
      printf("MASTER %d : Number of MPI tasks is %d\n",my_rank, num_procs);
      MPI_Get_processor_name(hostname, &len);
      printf ("Hello from task %d on %s!\n", my_rank, hostname);
   }
   /* determine partner and then send/receive with partner */
   if (my_rank < num_procs/2) {       // rank 0, and 1
     partner = num_procs/2 + my_rank; // rank 2+0, and 2+1

     sprintf(greeting, "SEND 1 : Greetings from process %d of %d!!!", my_rank, num_procs);
     
     MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, partner, tag, MPI_COMM_WORLD);
     MPI_Recv(messages, MAX_STRING, MPI_CHAR, partner, tag, MPI_COMM_WORLD, &status);
     }
   else if (my_rank >= num_procs/2) {  // rank 2, and 3
     partner = my_rank - num_procs/2;  // rank 2-2, and 3-2

     sprintf(greeting, "SEND 2 : Greetings from process %d of %d!!!", my_rank, num_procs);
    
     MPI_Recv(messages, MAX_STRING, MPI_CHAR, partner, tag, MPI_COMM_WORLD, &status);
     MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, partner, tag, MPI_COMM_WORLD);
     }

   /* print partner info and exit*/
   printf("Task %d is partner with %s\n",my_rank, messages);
}

  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;

}

