/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -std=C99 -o mpi_hello mpi_hello.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_hello
 *
 * Input:      
 *    None
/*
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
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* Include MPI header file that contains the library’s API */

const int MAX_STRING = 100;

int main(void) {                                    /* (argc, argv[]) = (NULL, NULL )*/
   char  greeting[MAX_STRING];                      /* String storing message,  char buffer */
   int   num_procs;                                 /* Number of processes  */
   int   my_rank;                                   /* My process rank  */

   MPI_Init (NULL, NULL);                           /* Initialize the MPI environment, starts MPI */
   MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);        /* Get current process' "ID" number (rank) */
   MPI_Comm_size (MPI_COMM_WORLD, &num_procs);      /* Get total number of processes */
   
/*==========================================================================================
AN EXAMPLE TO DEMONSTRATE A POINT-TO-POINT COMMUNICATION BY SENDING AND RECEIVING A STRING 
============================================================================================*/

   if (my_rank == 0) { /* Everyone, except process rank 0 */
      /* Create message, sprintf stands for “String print”. Instead of printing on console, it store output on char buffer which are specified in sprintf */
      sprintf(greeting, "SEND : Greetings from process %d of %d!!!", my_rank, num_procs);
      /* Send message to process 0 */
      for (int q = 1; q < num_procs; q++) { /* From rank 1 to (num_procs-1) */ 
         MPI_Send(greeting,           /* (I) initial address of send buffer (choice) */
                  strlen(greeting)+1, /* (I) number of elements in send buffer (nonnegative integer) */
                  MPI_CHAR,           /* (I) datatype of each send buffer element (handle) */
                  q,                  /* (I) rank of destination (integer) */ 
                  0,                  /* (I) message tag (integer) */ 
                  MPI_COMM_WORLD);    /* (I) communicator (handle) */
     } 
   } else {  
      /* On rank 0, print my message */
      printf("RECV : Greetings from process %d of %d!\n", my_rank, num_procs); 
      /* Receive message from process rank 1 to (num_procs-1) */
      MPI_Recv(greeting,           /* (O) initial address of receive buffer (choice) */
               MAX_STRING,         /* (I) maximum number of elements in receive buffer (integer)*/
               MPI_CHAR,           /* (I) datatype of each receive buffer element (handle)*/
               0,                  /* (I) rank of destination (integer)*/
               0,                  /* (I) message tag (integer)*/
               MPI_COMM_WORLD,     /* (I) communicator (handle) */ 
               MPI_STATUS_IGNORE); /* (O) status object (Status)*/
        

     /* Print reveive message from process rank q */
     printf("%s\n", greeting);

   }

  MPI_Finalize();                                  /* Finalize the MPI environment */

  return 0;
}
