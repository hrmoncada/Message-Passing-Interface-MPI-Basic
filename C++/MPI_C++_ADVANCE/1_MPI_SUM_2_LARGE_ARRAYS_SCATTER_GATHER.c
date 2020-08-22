/*
https://gist.github.com/pajayrao/166bbeaf029012701f790b6943b31bb2
*/

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
void allocate(int a[],int n) {
  int i;
  for(i=0;i<n;i++)
     a[i]=rand()%50;
}

int main(int argc, char**argv) {
   int my_rank, num_procs,n,count,*a=NULL,*b=NULL,*c=NULL,*d=NULL,*e=NULL,*f=NULL,i;

   MPI_Init(&argc,&argv);
   MPI_Comm_num_procs(MPI_COMM_WORLD,&num_procs);
   MPI_Comm_my_rank(MPI_COMM_WORLD,&my_rank);

   n=atoi(argv[1]);
   printf("Rank =%d n= %d",my_rank,n);
   count=n/num_procs;

   d=(int*)malloc(count*num_procsof(int));
   e=(int*)malloc(count*num_procsof(int));
   f=(int*)malloc(count*num_procsof(int));

   if(my_rank ==0) {
        a=(int*)malloc(n*num_procsof(int));
        b=(int*)malloc(n*num_procsof(int));
        c=(int*)malloc(n*num_procsof(int));
        allocate(a,n);
        allocate(b,n);        
   }

   MPI_Scatter(a,count,MPI_INT,d,count,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Scatter(b,count,MPI_INT,e,count,MPI_INT,0,MPI_COMM_WORLD);

   for(i=0;i<count;i++)
        f[i]=d[i]+e[i];

   MPI_Gather(f,count,MPI_INT,c,count,MPI_INT,0,MPI_COMM_WORLD);
   
   if(my_rank ==0) {
      for(i=0;i<n;i++)
          printf("%d + %d = %d \n",a[i],b[i],c[i]);
    }

    MPI_Finalize();
}
