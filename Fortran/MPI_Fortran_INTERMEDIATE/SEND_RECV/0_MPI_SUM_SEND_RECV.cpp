/* https://regi.tankonyvtar.hu/hu/tartalom/tamop412A/2011-0063_23_introduction_mpi/ar01s04.html
 parallel calculation of the sum from 1 to 10000
 
$ mpicxx 3_MPI_METHOD_3_MONTE_CARLO_SEND_RECV_GET_PI.c -o out
$ mpirun -np 4 ./out
I am the node 1; the partial sum is: 9376250
I am the node 3; the partial sum is: 21876250
I am the node 0; the partial sum is: 3126250
The sum yet is: 12502500
The sum yet is: 28128750
I am the node 2; the partial sum is: 15626250
The sum yet is: 50005000
The sum from 1 to 10000 is: 50005000
*/

#include <iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char ** argv){
  int id, nproc;
  int sum,startval,endval,accum;

  MPI_Init(&argc,&argv);   /* Initialize the MPI environment, starts MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);  /* Get total number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &id);      /* Get current process' "ID" number (rank) */ 
  MPI_Status status;

  sum = 0; // zero sum for accumulation
  startval = 10000*id/nproc+1;
  endval =   10000*(id+1)/nproc;

  for(int i=startval;i<=endval;++i)
    sum = sum + i;

  cout<<"I am the node "<< id;
  cout<< "; the partial sum is: "<< sum<<endl;

  if(id != 0) //the slaves sending back the partial sums
    MPI_Send(&sum,1,MPI_INT,0,1,MPI_COMM_WORLD);
  else //id==0! the master recieves the partial sums
    for(int j=1;j<nproc;j=j+1){
      MPI_Recv(&accum, 1, MPI_INT, j, 1, MPI_COMM_WORLD, &status);
      sum = sum + accum;
      cout<<"The sum yet is: "<<sum<<endl;
    }

  if(id == 0)
    cout << "The sum from 1 to 10000 is: " << sum << endl;

  MPI_Finalize();      /* Finalize the MPI environment */
}
