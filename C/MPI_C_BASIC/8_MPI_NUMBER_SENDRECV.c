/**** An example to demonstrate a Sendrecv communication  ***
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
#include "mpi.h"         /* Include MPI header file that contains the library’s API */

int main (int argc, char* argv[]) {
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;                               /* stores the status of whether a communication is successful or not */

  float a;
  int b;
  
  a = float(my_rank) * 11.1;
  b = my_rank;

  /*  MPI_Sendrecv - Sends and receives a message
  MPI_Sendrecv(const void *sendbuf,   // (I) initial address of send buffer (choice)
               int sendcount,         // (I) number of elements in send buffer (integer)
               MPI_Datatype sendtype, // (I) type of elements in send buffer (handle)
               int dest,              // (I) rank of destination (integer)
               int sendtag,           // (I) send tag (integer)
               void *recvbuf,         // (O) initial address of receive buffer (choice)
               int recvcount,         // (I) number of elements in receive buffer (integer)
               MPI_Datatype recvtype, // (I) type of elements in receive buffer (handle)
               int source,            // (I) rank of source (integer)
               int recvtag,           // (I) receive tag (integer)
               MPI_Comm comm,         // (I) communicator (handle)  
               MPI_Status *status)    // (O) status object (Status). This refers to the receive operation.
 */
  
  if (my_rank == 0)        // Rank 0 sends its a to Rank 1, and receives b
  {
    int partner_rank = 1;
    MPI_Sendrecv(&a, 
                 1,
                 MPI_FLOAT,
                 partner_rank,
                 0311,
                 &b,
                 1,
                 MPI_INT,
                 partner_rank,
                 0312,
                 MPI_COMM_WORLD,
                 &status);
    printf("I am Rank 0. Sent a=%f and received b=%d from Rank %d\n", a, b, status.MPI_SOURCE);
  }
  else if (my_rank == 1)   // Rank 1 sends its b to Rank 0, and receives a
  {
    int partner_rank = 0;
    MPI_Sendrecv(&b,
                 1,
                 MPI_INT,
                 partner_rank,
                 0312,
                 &a,
                 1,
                 MPI_FLOAT,
                 partner_rank,
                 0311,
                 MPI_COMM_WORLD,
                 &status);
    printf("I am Rank 1. Received a=%f and sent b=%d from Rank %d\n", a, b, status.MPI_SOURCE);
  }
  else                     // Other ranks sit and wait
    printf("I am Rank %d. I have nothing to do.\n", my_rank);


  // Print everything out to check
  printf("I am Rank %d. My a = %f and b = %d \n", my_rank, a, b);
 
  MPI_Barrier(MPI_COMM_WORLD);                   /* synchronize all processes */
  MPI_Finalize();                                /* finish up MPI */

  return 0;
}
