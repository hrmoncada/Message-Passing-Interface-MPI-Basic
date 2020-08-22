/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:  
 *   This program calculates pi using a "dartboard" algorithm.  
**********************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main (int argc, char *argv[]) {
double	homepi,     /* value of pi calculated by current task */
	pisum,	        /* sum of tasks' pi values */
	pi,	            /* average of pi after "darts" is thrown */
	avepi;	        /* average pi value for all iterations */
int rc,             /* return code */
	i;

/* Obtain number of tasks and task ID */
  int num_procs = 0;                              /* Number of processes  */
  int my_rank = 0;                                /* My process rank  */

  MPI_Init (&argc, &argv);                         /* Initialize the MPI environment, starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
  MPI_Status status;
  
/* Set seed for random number generator equal to task ID */
srandom (my_rank);

avepi = 0;
for (i = 0; i < ROUNDS; i++) {
   /* All tasks calculate pi using dartboard algorithm */
   homepi = dboard(DARTS);

   rc = MPI_Reduce(&homepi,        // (I) address of send buffer (choice)
                   &pisum,         // (O) address of receive buffer (choice, significant only at root)
                   1,              // (I) number of elements in send buffer (integer)
                   MPI_DOUBLE,     // (I) data type of elements of send buffer (handle)
                   MPI_SUM,        // (I) reduce operation (handle)
                   MASTER,         // (I) rank of root process (integer),that will receive the result of the reduction operation
                   MPI_COMM_WORLD);// (I) communicator (handle)
   
   /* Master computes average for this iteration and all iterations */
   if (my_rank == MASTER) {
      pi = pisum/num_procs;
      avepi = ((avepi * i) + pi)/(i + 1); 
      printf("   After %8d throws, average value of pi = %10.8f\n", (DARTS * (i + 1)),avepi);
   }    
}

if (my_rank == MASTER) 
   printf ("\nReal value of PI: 3.1415926535897 \n");

MPI_Finalize();       /* Finalize the MPI environment */  
return 0;
}

/**************************************************************************
* subroutine dboard
* DESCRIPTION:
*   Used in pi calculation example codes. 
*   See mpi_pi_send.c and mpi_pi_reduce.c  
*   Throw darts at board.  Done by generating random numbers 
*   between 0 and 1 and converting them to values for x and y 
*   coordinates and then testing to see if they "land" in 
*   the circle."  If so, score is incremented.  After throwing the 
*   specified number of darts, pi is calculated.  The computed value 
*   of pi is returned as the value of this function, dboard. 
*
*   Explanation of constants and variables used in this function:
*   darts       = number of throws at dartboard
*   score       = number of darts that hit circle
*   n           = index variable
*   r           = random number scaled between 0 and 1
*   x_coord     = x coordinate, between -1 and 1
*   x_sqr       = square of x coordinate
*   y_coord     = y coordinate, between -1 and 1
*   y_sqr       = square of y coordinate
*   pi          = computed value of pi
****************************************************************************/

double dboard(int darts) {
  #define sqr(x)    ((x)*(x))
  long random(void);
  double x_coord, y_coord, pi, r; 
  int score, n;
  unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
The cconst variable must be 4 bytes. We check this and bail if it is not the right size
 ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
   }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

   /* "throw darts at board" */
   for (n = 1; n <= darts; n++)  {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
           score++;
      }

/* calculate pi */
pi = 4.0 * (double)score/(double)darts;
return(pi);
} 
