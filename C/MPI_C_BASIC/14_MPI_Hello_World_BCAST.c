/*  https://www.d3s.mff.cuni.cz/legacy/~ceres/sch/mwy/text/ch07s09s05.html */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv) {

  int len;
  char *message;
  char gretting [] = "Hello World !";

  int num_procs = 0;                          /* Number of processes  */
  int my_rank = 0;                            /* My process rank  */
    
  MPI_Init (&argc, &argv);                    /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);   /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs); /* Get total number of processes */
  MPI_Status status;                          /* stores the status of whether a communication is successful or not */
  
  if (my_rank == 0) {
    len = sizeof (gretting);
    MPI_Bcast (&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (gretting, len, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf ("Process 0: Message sent\n");
  } else {
    MPI_Bcast (&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    message = (char *) malloc (len);
    MPI_Bcast (message, len, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf ("Process %d: %s\n", my_rank, message);
  }
  
  MPI_Finalize();                                /* Finalize the MPI environment */
  return 0;
}
