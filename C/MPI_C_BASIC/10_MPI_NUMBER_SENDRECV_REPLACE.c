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

int main (int argc, char* argv[]) {
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;                               /* stores the status of whether a communication is successful or not */

  int b;
  b = my_rank;

  int send_to_partner;
  int recv_from_partner;

  // Construct a ring communication  
  send_to_partner = my_rank - 1;                 /* send to a lower rank */
  recv_from_partner = my_rank + 1;               /* receive from an upper rank */
  if (my_rank == 0)                              /* take care of boundaries */
    send_to_partner = num_procs - 1;
  else if (my_rank == num_procs - 1)
    recv_from_partner = 0;
  
/* MPI_Sendrecv_replace - Sends and receives using a single buffer
   MPI_Sendrecv_replace(void* buf, int count, MPI_Datatype datatype,
                        int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
*/
  MPI_Sendrecv_replace(&b, 1,
                       MPI_INT,
                       send_to_partner,
                       my_rank,
                       recv_from_partner,
                       MPI_ANY_TAG,
                       MPI_COMM_WORLD,
                       &status);
  
  // Print everything out to check
  printf("Rank %d: b = %d \n", my_rank, b);
 
  MPI_Barrier(MPI_COMM_WORLD);                   /* Synchronize all processes */
  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;
}
