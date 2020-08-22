/* https://cvw.cac.cornell.edu/MPIcc/broadcast 
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

#include "mpi.h"     /* Include MPI header file that contains the library’s API */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* For strlen             */

const int MAX_STRING = 20;

int main(int argc, char **argv) {
  char greeting[MAX_STRING];                  /* String storing message, har buffer */ 
  int ROOT = 0;

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
  
  if (my_rank == ROOT) {
    strcpy(greeting,"Hello, world"); /* Copy the string pointed by source to the character array destination*/
  }    
  
  printf("From rank [%d]: Before Bcast, gretting is %s\n", my_rank, greeting);
  
  MPI_Bcast(greeting,
            13,
            MPI_CHAR,
            ROOT,
            MPI_COMM_WORLD);

  printf( "From rank [%d]: After Bcast: gretting is %s with length %zu\n", my_rank, greeting, strlen(greeting));

  MPI_Finalize();                                /* Finalize the MPI environment */
  return 0;
}
