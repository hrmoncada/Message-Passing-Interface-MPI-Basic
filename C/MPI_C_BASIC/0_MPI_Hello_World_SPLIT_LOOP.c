/* https://gist.github.com/bast/bd83baf2c69ea61914e6 */

#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[]) {
  int   num_procs;                                 /* Number of processes  */
  int   my_rank;                                   /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  
  int sum = 0;
  for (int i = 0; i < 17; i++) {
      if (i%num_procs != my_rank) continue;  //i divide by num_procs give as residue
      printf("rank %i working on element %i, sum = %d\n", my_rank, i, sum);
      // rest of work ..
         printf ("Hello world  - "); 
         sum = sum + i;
  }
  printf("\n");
  MPI_Finalize();                                /* Finalize the MPI environment */

  return 0;
}
