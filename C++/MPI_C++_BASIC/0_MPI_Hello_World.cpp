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
#include <mpi.h> /* Include MPI header file that contains the library’s API */
using namespace std;


int main (int argc, char **argv) {
  int my_rank, size;

  MPI_Init (&argc, &argv);         	      /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);  /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */

  cout << "with C++, Hello world, from process " << my_rank <<" of "<< size << endl;   // Printing

  MPI_Finalize();                         /* Finalize the MPI environment */

  return 0;
}
