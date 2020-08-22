/*
https://gist.github.com/pajayrao/166bbeaf029012701f790b6943b31bb2
*/

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <time.h>

int main(int argc,char **argv) {
    int my_rank,num_procs,i;
    double x=0,y=0,pi,z;
    int no = atoi(argv[1]);
    int count=0,total_count=0,no_div=0,fin_no = 0;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_my_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_num_procs(MPI_COMM_WORLD,&num_procs);
    
    no_div = no/num_procs;
    srand ( time(NULL) );
    
    for(i=0;i<no_div;i++) {
        x=(rand()%100)/(double)100;
        y=(rand()%100)/(double)100;
        z=x*x+y*y;
        if(z<=1)
            count++;

    }
    
    printf("For my_rank %d count = %d itrr = %d\n",my_rank,count,no_div);
    MPI_Reduce(&count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&no_div,&fin_no,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    
    if(my_rank ==0){
       printf("Total count  = %d, total itrr = %d\n",total_count,fin_no);
       pi = ((double)total_count)/fin_no*4.0000;
       printf("Pi value = %lf",pi);
    }
    
    MPI_Finalize();
}
