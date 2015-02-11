#include "ringgather.h"
void Gather_ring_nb(float* y, int blocksize, float* x)
{
    int rank, p;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    int i;


    for(i=0; i<blocksize; i++)
    {
        y[i+blocksize*rank]=x[i];
    }

    int suc, pred;
    int sendoff,recvoff;
    MPI_Request sreq,rreq;
    MPI_Status stat;
    suc = (rank+1)%p;
    pred = (rank-1+p)%p;
    sendoff = rank*blocksize;
    recvoff = ((rank-1+p)%p)*blocksize;

    for(i=0; i<p-1; i++)
    {
        MPI_Isend(y+sendoff,blocksize,MPI_FLOAT,suc ,0,MPI_COMM_WORLD,&sreq);
        MPI_Irecv(y+recvoff,blocksize,MPI_FLOAT,pred,0,MPI_COMM_WORLD,&rreq);

        sendoff=(rank-i-1+p)%p*blocksize;
        recvoff=(rank-i-2+p)%p*blocksize;

        MPI_Wait(&rreq,&stat);
        MPI_Wait(&sreq,&stat);
    }

}
void Gather_ring_sizes(int* sizes, int mysize)
{
    int rank, p;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    sizes[rank]=mysize;

    int suc, pred;
    int sendoff,recvoff;
    MPI_Request sreq,rreq;
    MPI_Status stat;
    suc = (rank+1)%p;
    pred = (rank-1+p)%p;
    sendoff = rank;
    recvoff = ((rank-1+p)%p);
    int i;
    for(i=0; i<p-1; i++)
    {
        MPI_Isend(sizes+sendoff,1,MPI_INT,suc ,0,MPI_COMM_WORLD,&sreq);
        MPI_Irecv(sizes+recvoff,1,MPI_INT,pred,0,MPI_COMM_WORLD,&rreq);

        sendoff=(rank-i-1+p)%p;
        recvoff=(rank-i-2+p)%p;

        MPI_Wait(&rreq,&stat);
        MPI_Wait(&sreq,&stat);
    }

}
void Gatherv_ring_nb(float* y, int* sizes, int* offsets, float* x)
{
    int p,rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    int i;
    for(i=0; i<sizes[rank]; i++)
    {
        y[offsets[rank]+i]=x[i];
    }
    int suc, pred;
    int sendoff,recvoff;
    MPI_Request sreq,rreq;
    MPI_Status stat;
    suc = (rank+1)%p;
    pred = (rank-1+p)%p;
    sendoff = rank;
    recvoff = ((rank-1+p)%p);
    for(i=0; i<p-1; i++)
    {
        MPI_Isend(y+offsets[sendoff],sizes[sendoff],MPI_FLOAT,suc ,0,MPI_COMM_WORLD,&sreq);
        MPI_Irecv(y+offsets[recvoff],sizes[recvoff],MPI_FLOAT,pred,0,MPI_COMM_WORLD,&rreq);

        sendoff=(rank-i-1+p)%p;
        recvoff=(rank-i-2+p)%p;

        MPI_Wait(&rreq,&stat);
        MPI_Wait(&sreq,&stat);
    }


}

void Sizes_count_offsets(int* res, int* sizes, int count)
{
    int i;
    res[0]=0;
    for(i=1; i<count; i++)
    {
        res[i]=res[i-1]+sizes[i-1];
    }
}


