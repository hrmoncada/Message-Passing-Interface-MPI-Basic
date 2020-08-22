/* Calculation of the constant Pi by Monte Carlo method
$ mpicxx 4_MPI_METHOD_3_MONTE_CARLO_SEND_RECV_GET_PI.cpp -o out
$ mpirun -np 4 ./out
Pi: 		3.14159265359
Pi by MC: 	3.141614872
Error: 		0.000022218410

*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv){
  int id, nproc;
  MPI_Status status;
  double x,y, Pi, error;
  long long allsum;
  const long long iternum=1000000000;

  // Initialize MPI:
  MPI_Init(&argc, &argv);
  // Get my rank:
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  // Get the total number of processors:
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  srand((unsigned)time(0));
  cout.precision(12);

  long long sum=0;
  for(long long i=0;i<iternum;++i){
    x=(double)rand()/RAND_MAX;
    y=(double)rand()/RAND_MAX;
    if(x*x+y*y<1) ++sum;
  }

  //Slave:
  if(id!=0){
    MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD);
  }

  //Master:
  else{
    allsum=sum;
    for(int i=1;i<nproc;++i){
      MPI_Recv(&sum, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
	       &status);
      allsum+=sum;
    }

    //calculate Pi, compare to the Pi in math.h
    Pi=(4.0*allsum)/(iternum*nproc);
    error = fabs( Pi-M_PI );
    cout<<"Pi: \t\t"<<M_PI<<endl;
    cout<<"Pi by MC: \t"<<Pi<<endl;
    cout<<"Error: \t\t"<<fixed<<error<<endl;
  }

  // Terminate MPI:
  MPI_Finalize();
  return 0;
}
