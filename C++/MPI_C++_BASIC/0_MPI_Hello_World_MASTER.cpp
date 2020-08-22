/*
Compile
mpicxx 0_MPI_HelloWorld.cpp -o out
or
mpic++ 0_MPI_HelloWorld.cpp -o out

Execute:
mpirun -n 4 ./out
or
mpirun -np 4 out

Hello from process 0 of 4
Hello from process 1 of 4
Hello from process 2 of 4
Hello from process 3 of 4
*/

#include <iostream>
#include "mpi.h"                /* Include MPI header file that contains the library’s API */
using namespace std;
#define  MASTER 0               /* my_rank 0 is the MASTER */

int main (int argc, char *argv[]) {
  int  my_rank, num_procs, len;
  char hostname[MPI_MAX_PROCESSOR_NAME];           /* Processor name, This must be an array of size at least MPI_MAX_PROCESSOR_NAME. */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  
  MPI_Get_processor_name(hostname, &len);          /* Gets the name of the processor*/

  cout << "Hello from task " << my_rank << " on "<< hostname << " with hostname size "<< len << endl;
  
  if (my_rank == MASTER)
     count << "MASTER " << my_rank << ": Number of MPI tasks is:" <<  num_procs << endl;
  
  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;

}

