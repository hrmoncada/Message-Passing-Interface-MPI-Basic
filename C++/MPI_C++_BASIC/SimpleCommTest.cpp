/* https://faculty.csbsju.edu/mheroux/fall2011_csci317/SimpleCommTest.cpp.html */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
// Only header file needed for MPI
#include <mpi.h>

/**====================================================================**/
int main(int argc, char **argv) {
  
  // The program demonstrates basic use of MPI send and receive functions.
  // We time communication costs for both long messages (1M bytes) and 
  // zero length messages.  This gives us a measure of the asymptotic 
  // bandwidth and the latency for the given machine.
  
  // We are using the C interface to MPI, so it is natural to declare most 
  // variable here, since we will be using them in C-like statements.
  
  int count, nCount1 = 50, nCount2 = 10000, num_procs, my_rank;
  int master_rank = 0, MPITag = 99, msgSize;
  MPI_Status status;
  
  //  Set up timers.
  double time1, time2, time0, latency, bandwidth, loopTime;
  
  // declare an array to send back and forth
  double a[125000]; // 1 million bytes (8 bytes per double)
  
  // initialize the array
  for(int i=0;i<125000;i++)a[i] = 1.0;
  
  //** Perform MPI initialization.
  // Pass input args to MPI_Init in case we want to change MPI behavior
  MPI_Init (&argc, &argv); 
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs); // Get number of procs
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); // Get my rank
  
  // This is hard-wired to run on two processors.
  if (num_procs!=2 && my_rank==0) {
    cerr << "This program must be run on 2 MPI procs." << endl;
    abort;
  }
  
  cout << "Processor " << my_rank << " is active." << endl << flush;
  MPI_Barrier(MPI_COMM_WORLD);
  
  // Now perform the 1M message tests.
  //  Start clock timer.
  if(my_rank == master_rank)
    cout << "Doing " << nCount1 << " send/recv (1MB) pairs... " << endl;
  time0 = MPI_Wtime ();
  
  cout << endl;
  for(count=0;count<nCount1;count++){
    if (my_rank == master_rank) {
      MPI_Send (a, 125000, MPI_DOUBLE, 1, MPITag, 
                MPI_COMM_WORLD);
      MPI_Recv (a, 125000, MPI_DOUBLE, 1, MPITag, 
                MPI_COMM_WORLD, &status);
      cout << "." << flush;
    } 
    else {
      MPI_Recv (a, 125000, MPI_DOUBLE, master_rank, MPITag, 
                MPI_COMM_WORLD, &status);
      MPI_Send (a, 125000, MPI_DOUBLE, master_rank, MPITag, 
                MPI_COMM_WORLD);
    }
  }
  if (my_rank == master_rank) cout << endl;
  //  check clock timer.
  time1 = MPI_Wtime ();
  
  
  MPI_Barrier(MPI_COMM_WORLD);
  if(my_rank == master_rank){
    time2 = time1 - time0;
    msgSize = 125000*sizeof(double);
    bandwidth = 2.0*nCount1/time2*(1.0*msgSize/1.e6);
    cout << "Time for " << nCount1 << " send/recv pairs: " << time2 << endl;
    cout << "Bandwidth for 1MB messages (MB/sec): " << bandwidth << endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (my_rank == master_rank) cout << endl;
  //  Start clock timer.
  if(my_rank == master_rank)
    cout << "Doing " << nCount2 << " 0-byte send/recv pairs... " << endl;
  
  // Compute overhead do to looping time (will subtract below)
  loopTime = MPI_Wtime();
  for(count=0;count<nCount2;count++){
    if (my_rank == master_rank) continue;
    else continue;
  }
  loopTime = MPI_Wtime() - loopTime;
  
  time0 = MPI_Wtime ();
  for(count=0;count<nCount2;count++){
    if (my_rank == master_rank) {
      MPI_Send (a, 0, MPI_DOUBLE, 1, MPITag, 
                MPI_COMM_WORLD);
      MPI_Recv (a, 0, MPI_DOUBLE, 1, MPITag, 
                MPI_COMM_WORLD, &status);
    } 
    else {
      MPI_Recv (a, 0, MPI_DOUBLE, master_rank, MPITag, 
                MPI_COMM_WORLD, &status);
      MPI_Send (a, 0, MPI_DOUBLE, master_rank, MPITag, 
                MPI_COMM_WORLD);
    }
  }
  //  check clock timer.
  time1 = MPI_Wtime ();
  
  MPI_Barrier(MPI_COMM_WORLD);
  if(my_rank == master_rank){
    time2 = time1 - time0;
    latency = (time2-loopTime)/(2.0*nCount2);
    cout << "Time for " << nCount2 << " 0-byte send/recv pairs: " << time2 
    << endl;
    cout << "Latency (micro-sec): " << latency*1.e6 << endl;
  }
  MPI_Finalize();
  
  return(0);
}
