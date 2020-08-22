/*
https://gist.github.com/pajayrao/166bbeaf029012701f790b6943b31bb2
*/
#include<mpi.h>
#include<stdio.h>

int main(int argc,char **argv) {
   int my_rank, num_procs;
   char A[3][50]={"RVCE","COLLEGE","SIDDA"};
   char B[50]={},C[50]={},D[50]={};
   
   MPI_Init(&argc,&argv);
   MPI_Comm_my_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_num_procs(MPI_COMM_WORLD,&num_procs);
   
   if(my_rank==0) {
      printf("Rank %d started \n",my_rank);
   
      MPI_Recv(B,50,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Rank %d recieve %s message \n",1,B);
      
      MPI_Recv(C,50,MPI_CHAR,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Rank %d recieve %s message \n",2,C);
      
      MPI_Recv(D,50,MPI_CHAR,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Rank %d recieve %s message \n",3,D);
   } else {
      printf("Rank %d sends %s message \n",my_rank,A[my_rank-1]);
      MPI_Send(A[my_rank-1],20,MPI_CHAR,0,0,MPI_COMM_WORLD);
   }
   
   MPI_Finalize();
}
