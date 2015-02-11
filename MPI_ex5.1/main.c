#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "string.h"

char* tc(int i,char* target)
{
    target[0]=48;
    int j=0;
    while(i)
    {
        target[j]=i%10+48;
        i/=10;
        j++;
    }
}

int main(int argc, char* argv[])
{
    int rank, p, size=8;
    char* num;
    int left, right;
    char send_buffer1[8], recv_buffer1[8];
    char send_buffer2[8], recv_buffer2[8];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, & rank);
    MPI_Comm_size(MPI_COMM_WORLD, & p);
    left = (rank-1 + p) % p;
    right = (rank+1) % p;
    if(left==right) MPI_Finalize();
    strcpy(send_buffer1,"ring");
    tc(rank,send_buffer1+4);

    strcpy(send_buffer2,send_buffer1);
    MPI_Status stat;
    /*
    MPI_Sendrecv(send_buffer1,size,MPI_CHAR,left,0,recv_buffer1,size,MPI_CHAR,right,0,MPI_COMM_WORLD,&stat);
    MPI_Sendrecv(send_buffer2,size,MPI_CHAR,right,1,recv_buffer2,size,MPI_CHAR,left,1,MPI_COMM_WORLD,&stat);
    */
    /*
    switch(rank%2)
    {
    case 0:
        MPI_Send(send_buffer1, size, MPI_CHAR, left,0,MPI_COMM_WORLD);
        MPI_Recv(recv_buffer1, size, MPI_CHAR, right, 0, MPI_COMM_WORLD, &stat);

        MPI_Send(send_buffer2, size, MPI_CHAR, right, 1, MPI_COMM_WORLD);
        MPI_Recv(recv_buffer2, size, MPI_CHAR, left, 1, MPI_COMM_WORLD, &stat);
        break;
    case 1:

        MPI_Recv(recv_buffer1, size, MPI_CHAR, right, 0, MPI_COMM_WORLD, &stat);
        MPI_Send(send_buffer1, size, MPI_CHAR, left,0,MPI_COMM_WORLD);

        MPI_Recv(recv_buffer2, size, MPI_CHAR, left, 1, MPI_COMM_WORLD, &stat);
        MPI_Send(send_buffer2, size, MPI_CHAR, right, 1, MPI_COMM_WORLD);
        break;
    }
    */
    MPI_Request req,req1,req2;

    MPI_Isend(send_buffer1,size,MPI_CHAR,left,0,MPI_COMM_WORLD,&req);
    MPI_Isend(send_buffer2,size,MPI_CHAR,right,1,MPI_COMM_WORLD,&req);

    MPI_Irecv(recv_buffer1,size,MPI_CHAR,left,1,MPI_COMM_WORLD,&req1);
    MPI_Irecv(recv_buffer2,size,MPI_CHAR,right,0,MPI_COMM_WORLD,&req2);

    MPI_Wait(&req1,&stat);
    MPI_Wait(&req2,&stat);

    printf("%s %s %s\n",send_buffer1,recv_buffer1,recv_buffer2);

    MPI_Finalize();
}
