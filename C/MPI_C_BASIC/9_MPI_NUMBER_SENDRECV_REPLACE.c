/*** An example to demonstrate a point-to-point Sendrecv_replace communication  ***
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
#include "mpi.h"       /* Include MPI header file that contains the library’s API */
       
int main (int argc, char* argv[]){
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;                               /* stores the status of whether a communication is successful or not */
  
  /* MPI_Sendrecv_replace - Sends and receives using a single buffer
   * MPI_Sendrecv_replace(void* buf,           // (O) initial address of send and receive buffer (choice)
                         int count,            // (I) number of elements in send and receive buffer (integer)
                         MPI_Datatype datatype,// (I) type of elements in send and receive buffer (handle)
                         int dest,             // (I) rank of destination (integer)
                         int sendtag,          // (I) send message tag (integer)
                         int source,           // (I) rank of source (integer)
                         int recvtag,          // (I) receive message tag (integer)
                         MPI_Comm comm,        // (I) communicator (handle)
                         MPI_Status *status)   // (O) status object (Status) */
  int b[10];
  for (unsigned int i=0; i<10; ++i)
      b[i] = my_rank * i;

  if (my_rank == 0) {
    int partner_rank = 1;
    
    MPI_Sendrecv_replace(&b,
                         10,
                         MPI_INT,
                         partner_rank,
                         0311,
                         partner_rank,
                         0312,
                         MPI_COMM_WORLD,
                         &status);
    
    printf("I am Rank 0. Exchanged array b with Rank %d\n", status.MPI_SOURCE);
  } else if (my_rank == 1) {
    int partner_rank = 0;
    
    MPI_Sendrecv_replace(&b,
                         10,
                         MPI_INT,
                         partner_rank,
                         0312,
                         partner_rank,
                         0311,
                         MPI_COMM_WORLD,
                         &status);
    printf("I am Rank 1. Exchanged array b with Rank %d\n", status.MPI_SOURCE);
  } else                     // Other ranks sit and wait
    printf("I am Rank %d. I have nothing to do.\n", my_rank);

  // Print everything out to check
  for (unsigned int i=0; i<10; ++i)
    printf("Rank %d: b[%d] = %d \n", my_rank, i, b[i]);
 
  MPI_Barrier(MPI_COMM_WORLD);                   /* Synchronize all processes */
  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;
}
