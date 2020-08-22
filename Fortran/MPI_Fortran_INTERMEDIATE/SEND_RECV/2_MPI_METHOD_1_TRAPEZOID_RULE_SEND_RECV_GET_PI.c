/* http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/92-MPI/send+recv.html 
 
$ mpicc 2_MPI_METHOD_1_TRAPEZOID_RULE_SEND_RECV_GET_PI.c -o out -lm
$ mpirun -np 4 ./out 1000
Done... Now collecting output from others
Pi fraction received from proc 1 = 0.766135
Pi fraction received from proc 2 = 0.779321
Pi fraction received from proc 3 = 0.811751
Pi = 3.11454

Elapsed time = 287 microseconds
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

struct timeval start_time, stop_time;
int elapsed;
int num_procs;

double f(double a) {
   return( sqrt(1 - a) );
}

int main(int argc, char *argv[]) {
   int    i, N;
   double h, h2, x;
   double mypi = 0.0, others_pi, sum = 0.0;
   double begin = 0.0, end = 0.0;
   double PI25DT = 3.141592653589793238462643;
   
   int  my_rank = 0;                           /* My process rank  */
   
   MPI_Init(&argc,&argv);                      /* Initialize the MPI environment, starts MPI */
   MPI_Comm_size(MPI_COMM_WORLD, &num_procs);  /* Get total number of processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    /* Get current process' "ID" number (rank) */

 //Synchronize all processes and get the begin time
  MPI_Barrier(MPI_COMM_WORLD);
  
   // Returns an elapsed time on the calling processor 
  begin = MPI_Wtime();
   
   if ( argc != 2 ) {
      if ( my_rank == 0 ) {     
          printf("Usage: %s   Num_Intervals\n", argv[0]);
      }   
      MPI_Finalize();                           /* Finalize the MPI environment */
      exit(1);
   }
   
   N = atoi(argv[1]);
   h = 1.0/(double) N;
   h2 = h * h 
   ;
   for (i = my_rank; i < N; i = i + num_procs) {
      x = h2 * i * i ;
      sum +=  f(x);  
   }
   
/* Get the partial sum from others and form the total */
   if ( my_rank == 0 ) {
      printf("Done!! ... Now collecting output from others\n");  
      printf("Pi fraction received from proc %d  = %f\n", my_rank, sum);

      for (i = 1; i < num_procs; i++) {
         MPI_Recv(&others_pi, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL);
         printf("Pi fraction received from proc %d  = %f\n", i, others_pi);
         sum += others_pi;
      }
      mypi = 4 * h * sum;
      
// Returns an elapsed time on the calling processor 
      end = MPI_Wtime();
    
      printf("Time = %fs;    PI = %lf\n", end-begin, mypi);
      printf("PI is approximately %.16f, Error is %.16f\n", mypi, fabs(mypi - PI25DT));
      
   } else { // everyone differet from my_rank = 0
      MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
   }

   MPI_Finalize();                                 /* Finalize the MPI environment */
}
