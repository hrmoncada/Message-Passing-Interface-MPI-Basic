/* https://www.d3s.mff.cuni.cz/legacy/~ceres/sch/mwy/text/ch07s09s05.html */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char* argv[]) {
  int iOutput;
  int iInput;
  int num_procs;                                 /* Number of processes  */
  int my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */

  if (my_rank == 0) iInput = 1;
  else iInput = my_rank;

  MPI_Scan (&iInput, &iOutput, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

  printf ("Process %d: Factorial %d\n", my_rank, iOutput);
  
  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;
}
