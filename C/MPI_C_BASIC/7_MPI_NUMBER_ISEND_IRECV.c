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
#include "mpi.h"               /* Include MPI header file that contains the library’s API */
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[]) {
  int   num_procs;                        /* Number of processes  */
  int   my_rank;                          /* My process rank  */
  int   len;                              /* Length (in characters) of the hostname */
  char hostname[MPI_MAX_PROCESSOR_NAME];  /* A unique specifier of the actual node. This must be an array of size at least MPI_MAX_PROCESSOR_NAME.*/
  
  int  partner, message;
  
  MPI_Status stats[2];  /* stores the status of whether a communication is successful or not */
  MPI_Request reqs[2];  /* A null handle, communication request (handle) */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */

  /* NONBLOCKING send and receive, the routine is not interrupt safe
   * MPI_Isend(const void *buf,     // (I) initial address of send buffer (choice)
              int count,            // (I) number of elements in send buffer (integer)
              MPI_Datatype datatype,// (I) datatype of each send buffer element (handle)
              int dest,             // (I) rank of destination (integer)
              int tag,              // (I) message tag (integer)
              MPI_Comm comm,        // (I) communicator (handle)
              MPI_Request *request);// (I) communication request (handle)
          
  MPI_Irecv(void *buf,              // (I)initial address of receive buffer (choice)
              int count,            // (I) number of elements in receive buffer (integer)
              MPI_Datatype datatype,// (I) datatype of each receive buffer element (handle)
              int source,           // (I) rank of source (integer)
              int tag,              // (I) message tag (integer)
              MPI_Comm comm,        // (I) communicator (handle)
              MPI_Request *request);// (O) communication request (handle) */ 
/* need an even number of tasks  */
  if (num_procs % 2 != 0) {
     if (my_rank == MASTER)
         printf("Quitting. Need an even number of tasks: num_procs=%d\n", num_procs);
  } else {
     MPI_Get_processor_name(hostname, &len);
     printf ("Hello from task %d on %s!\n", my_rank, hostname);
     
     if (my_rank == MASTER)
        printf("MASTER: Number of MPI tasks is: %d\n",num_procs);

   /* determine partner and then send/receive with partner */
     if (my_rank < num_procs/2)          // rank 0, and 1
        partner = num_procs/2 + my_rank; // rank 2+0, and 2+1
     else if (my_rank >= num_procs/2)    // rank 2, and 3
        partner = my_rank - num_procs/2; // rank 2-2, and 3-2

     MPI_Irecv(&message,
               1,
               MPI_INT,
               partner,
               1,
               MPI_COMM_WORLD,
               &reqs[0]);
     
     MPI_Isend(&my_rank,
               1,
               MPI_INT,
               partner,
               1,
               MPI_COMM_WORLD,
               &reqs[1]);

   /* now block until requests are complete */
     MPI_Waitall(2, reqs, stats);

   /* print partner info and exit*/
     printf("Task %d is partner with %d\n",my_rank,message);
  }

   MPI_Finalize();                                /* Finalize the MPI environment */

}

