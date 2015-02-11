#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char* argv[])
{

    int rank,p;
    void* ptr=0;
    int size_min=0, size_max=1024*1024*128, size_step=1024*1024, tests=10;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    double t1, t2;
    double total=0, avg=0;
    int i,j;
    size_min+=size_step;
    MPI_Request sreq,rreq;
    MPI_Status stat;
    ptr=malloc(1);
    if(rank==0)
    {
        for(i=0; i<size_max/size_step; i++)
        {
            free(ptr);
            ptr=malloc(size_min);
            total = 0;
            avg=0;
            printf("Ping test for %i bytes of data.\n",size_min);
            for(j=0; j<tests; j++)
            {
                t1 = MPI_Wtime();

                MPI_Send(ptr,size_min,MPI_BYTE,1,0,MPI_COMM_WORLD);
                MPI_Recv(ptr,size_min,MPI_BYTE,1,0,MPI_COMM_WORLD,&stat);

                t2 = MPI_Wtime();
                printf("%f ",t2-t1);
                total+=t2-t1;


            }
            avg=total/tests;
            printf("\n");
            printf("Total=%f Avg=%f\n",total,avg);

            size_min+=size_step;
        }
    }
    else if (rank==1)
    {
        for(i=0; i<size_max/size_step; i++)
        {
            free(ptr);
            ptr=malloc(size_min);
            total = 0;
            avg=0;
            for(j=0; j<tests; j++)
            {
                t1 = MPI_Wtime();

                MPI_Recv(ptr,size_min,MPI_BYTE,0,0,MPI_COMM_WORLD,&stat);
                MPI_Send(ptr,size_min,MPI_BYTE,0,0,MPI_COMM_WORLD);

                t2 = MPI_Wtime();
                total+=t2-t1;

            }
            avg=total/tests;
            size_min+=size_step;
        }
    }
    else
    {
        MPI_Finalize();
        return 0;
    }

    MPI_Finalize();

    return 0;
}
