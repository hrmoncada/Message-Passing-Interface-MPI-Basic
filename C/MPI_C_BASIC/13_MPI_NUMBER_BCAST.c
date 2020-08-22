/* http://selkie.macalester.edu/csinparallel/modules/Patternlets/build/html/MessagePassing/Broadcast.html 
broadcast.c
 * ... illustrates the use of MPI_Bcast() with a scalar value...
 *      (compare to array version).
 * Joel Adams, Calvin College, April 2016.
 *
 * Usage: mpirun -np N ./broadcast
 *
 * Exercise:
 * - Compile and run several times,
 *     using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 *     (noting contents of file "data.txt");
 * - Explain behavior/effect of MPI_Bcast().
 
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
#include <string.h>  /* For string length             */
#include <assert.h>  /* Used to verify assumptions made by the program and print a diagnostic message if this assumption is false. */

int main(int argc, char** argv) {
   int answer = 0;
   int num_procs = 0;                          /* Number of processes  */
   int my_rank = 0;                            /* My process rank  */
    
   MPI_Init (&argc, &argv);                    /* Initialize the MPI environment, starts MPI */
   MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);   /* Get current process' "ID" number (rank) */
   MPI_Comm_size (MPI_COMM_WORLD, &num_procs); /* Get total number of processes */
   
   if (my_rank == 0) {
      FILE *fp = fopen("data.txt", "r");
      assert(fp != NULL );
      fscanf(fp, " %d", &answer);
      fclose(fp);
   }

   printf("BEFORE the broadcast, process %d's answer = %d\n", my_rank, answer);

   MPI_Bcast(&answer, 1, MPI_INT, 0, MPI_COMM_WORLD);

   printf("AFTER the broadcast, process %d's answer = %d\n", my_rank, answer);

   MPI_Finalize();                                /* Finalize the MPI environment */
    
   return 0;
}
