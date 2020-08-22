/* http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/92-MPI/send+recv.html 
 
$ mpicxx 1_MPI_METHOD_1_TRAPEZOID_RULE_SEND_RECV_GET_PI.cpp -o out
$ mpirun -np 4 ./out 1000
Done... Now collecting output from others
Pi fraction received from proc 1 = 0.766135
Pi fraction received from proc 2 = 0.779321
Pi fraction received from proc 3 = 0.811751
Pi = 3.11454

Elapsed time = 287 microseconds
 */
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>
using namespace std;

struct timeval start_time, stop_time;
int elapsed;
int num_procs;

double f(double a) {
   return( 2.0 / sqrt(1 - a*a) );
}

int main(int argc, char *argv[]) {
   int    N;
   double w, x;
   int    i, myid;
   double mypi, others_pi;

   MPI_Init(&argc,&argv);                      /* Initialize the MPI environment, starts MPI */
   MPI_Comm_size(MPI_COMM_WORLD, &num_procs);  /* Get total number of processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);       /* Get current process' "ID" number (rank) */

   /* -----
      Check command line
      ----- */
   if ( argc != 2 ) {
      if ( myid == 0 )
         cout << "Usage: " << argv[0] << " Num_Intervals" << endl;

      MPI_Finalize();                           /* Finalize the MPI environment */
      exit(1);
   }

   if ( myid == 0 )
      gettimeofday(&start_time, NULL);

   /* ******************************************************************* */
   N = atoi(argv[1]);
   w = 1.0/(double) N;

   mypi = 0.0;

   for (i = myid; i < N; i = i + num_procs) {
      x = w*(i + 0.5);         
      mypi = mypi +  w*f(x);  
   }

   /* ******************************************************************* */
   if ( myid == 0 ) {
      cout << "Done... Now collecting output from others" << endl;

      /* --------------------------------------------------------
         Get the partial sum from others and form the total
         -------------------------------------------------------- */
      for (i = 1; i < num_procs; i++) {

         MPI_Recv(&others_pi, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL);
         cout << "Pi fraction received from proc " << i << " = " 
			<< others_pi << endl;

         mypi += others_pi;
      }

      gettimeofday(&stop_time, NULL);

      elapsed = (stop_time.tv_sec*1000000 + stop_time.tv_usec) -
                (start_time.tv_sec*1000000 + start_time.tv_usec);

      cout << "Pi = " << mypi<< endl << endl;
      cout << "Elapsed time = " << elapsed << " microseconds" << endl;
   } else {
      MPI_Send(&mypi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
   }

   MPI_Finalize();                                 /* Finalize the MPI environment */
}
