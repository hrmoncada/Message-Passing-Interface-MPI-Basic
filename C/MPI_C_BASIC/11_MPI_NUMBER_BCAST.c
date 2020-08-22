/* https://stackoverflow.com/questions/7864075/using-mpi-bcast-for-mpi-communication 
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
#include "mpi.h"                     /* Include MPI header file that contains the library’s API */

#define  ROOT 0

int main(int argc, char** argv) {
  int buffer;                                 /* buffer */
  int num_procs = 0;                          /* Number of processes  */
  int my_rank = 0;                            /* My process rank  */
  
  MPI_Init (&argc, &argv);                    /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);   /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs); /* Get total number of processes */
  MPI_Status status;                          /* stores the status of whether a communication is successful or not */

/* MPI_Bcast - Broadcasts a message from the process with rank "root" to all other processes of the communicator
MPI_Bcast( void *buffer,         // (I/0) starting address of buffer (choice)
           int count,            // (I) number of entries in buffer (integer)
           MPI_Datatype datatype,// (I) data type of buffer (handle)
           int root,             // (I) rank of broadcast root (integer)
           MPI_Comm comm );      // (I) communicator (handle)
*/   
   if (my_rank == ROOT) { /* my_rank 0 has the buffer */
      buffer = 777;
   }

   printf("From rank [%d]: Before Bcast, buffer is %d\n", my_rank, buffer);

/* everyone calls bcast, data is taken from root and ends up in everyone's buffer */
   MPI_Bcast(&buffer,
             1,
             MPI_INT,
             ROOT,
             MPI_COMM_WORLD);

   printf("From rank [%d]: After Bcast, buffer is %d\n", my_rank, buffer);

   MPI_Finalize();                                /* Finalize the MPI environment */
   return 0;
}
