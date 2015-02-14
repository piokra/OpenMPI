#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "local_matrix.h"
#include "seq_gauss.h"
#include <string.h>
#define ABS(a) (((a)<0)?(-(a)):((a)))
/** MATRIX INDEXES
    (y,x)

    0,0 0,1 0,2 ...
    1,0 1,1 ...
    2,0 ...
    ...

                    **/



typedef struct DOUBLE_INT
{
    double val;
    int it;
}DOUBLE_INT;

int get_row_owner(int row);
int get_owned_rows(int rank, int size);

void scatter_data_rc(double** matrix, double* vec, int size);
void bcast_vec(double* vec, int size, int root);

double* parrarel_gauss_rc(double** matrix, double* vec, int size);
void add_row(double** matrix, double* vec, int size, int target, double mul, int k);

DOUBLE_INT get_local_max(double** matrix, int k, int size);
int get_global_max(double** matrix, int root, int k, int size);
void exchange_vectors(double** matrix, double* vec, int src, int dest, int size);

int main(int argc, char* argv[])
{

    int rank,j,p,size=1000,i;
    double seq_s,seq_e;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    /** Code goes here **/
    double* vec = 0;
    double* rvec = 0;
    double** matrix = 0;
    if(!rank)
    {

        load_vector("vec.dat",&vec,&size);
        load_matrix("matrix.dat",&matrix,&size,&size);

    }

    MPI_Bcast(&size,1,MPI_INT,0,MPI_COMM_WORLD);
    if(rank)
    {

        vec = malloc(sizeof(double)*(size/p+1));
        matrix=allocate_matrix(size/p+1,size);

    }


    //if(!rank) display_matrix(matrix,size,size);


    seq_s=MPI_Wtime();

    scatter_data_rc(matrix,vec,size);
    rvec=parrarel_gauss_rc(matrix,vec,size);

    seq_e=MPI_Wtime();
    printf("Work time=%f\n",seq_e-seq_s);
    MPI_Barrier(MPI_COMM_WORLD);

    double res=0;
    if(!rank)
    {
        save_vector("result.dat",rvec,size);
    }
    MPI_Finalize();

    return 0;



}
int get_row_owner(int row) //works
{
    int p;
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    return row%p;
}

int get_owned_rows(int rank, int size)
{
    int p;
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    if(size%p<=rank) return size/p;
    return size/p+1;

}



void scatter_data_rc(double** matrix, double* vec, int size) // works
{
    int rank,p;
    int i=0,j=0;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);


    if(rank==0)
    {
        MPI_Request req[2*(p-1)];
        MPI_Status stats[2*(p-1)];

        for(i=0; i<size; i++)
        {
            if(i%p)
            {
                MPI_Isend(matrix[i],size,MPI_DOUBLE,i%p,0,MPI_COMM_WORLD,&req[i%p-1]);
                MPI_Isend(vec+i,1,MPI_DOUBLE,i%p,0,MPI_COMM_WORLD,&req[i%p+p-2]);
            }
        }
        MPI_Waitall(2*(p-1),req,stats);
        for(i=1; i<get_owned_rows(0,size); i++)
        {

                matrix[i]=matrix[i*p];
                vec[i]=vec[i*p];

        }
    }
    else
    {
        MPI_Status stat;
        for(i=0;i<size; i++)
        {
            if(i%p==rank)
            {
                MPI_Recv(matrix[j],size,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&stat);
                MPI_Recv(vec+j,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&stat);
                j++;
            }
        }
    }
}

void bcast_vec(double* vec, int size, int root) //works
{
    MPI_Bcast(vec,size,MPI_DOUBLE,root,MPI_COMM_WORLD);
}
void add_row(double** matrix, double* vec, int size, int target, double mul, int k) // works
{
    int i,p,rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    for(i=k;i<size; i++)
        matrix[target][i]+=mul*vec[i];
}

DOUBLE_INT get_local_max(double** matrix, int k, int size)
{
    int rank,p;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    DOUBLE_INT di;
    int i,it=0;
    double t=0;
    for(i=(k-rank)/p;i<get_owned_rows(rank,size);i++)
    {
        if(i*p+rank>=k)
        if(ABS(matrix[i][k])>t)
        {
            t=ABS(matrix[i][k]);
            it=i*p+rank;
        }
    }
    di.it=it;
    di.val=t;
    return di;
}

int get_global_max(double** matrix, int root, int k, int size)
{
    int rank,p;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    DOUBLE_INT di;
    DOUBLE_INT recv;
    di = get_local_max(matrix,k,size);
    MPI_Reduce(&di,&recv,1,MPI_DOUBLE_INT,MPI_MAXLOC,root,MPI_COMM_WORLD);
    return recv.it;

}
void exchange_vectors(double** matrix, double* vec, int src, int dest, int size)
{
    if(src==dest) return;
    int i,rank,p,tag=0;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    double recv[size];
    double t;
    MPI_Status stat;
    if(get_row_owner(dest)==get_row_owner(src))
    {
        if(get_row_owner(src)==rank)
        {

            t=vec[src/p];
            vec[src/p]=vec[dest/p];
            vec[dest/p]=t;
            for(i=0; i<size; i++)
            {
                recv[i]=matrix[src/p][i];
                matrix[src/p][i]=matrix[dest/p][i];
                matrix[dest/p][i]=recv[i];
            }
        }
        return;
    }
    if(get_row_owner(src)==rank)
    {
        MPI_Send(matrix[src/p],size,MPI_DOUBLE,get_row_owner(dest),tag,MPI_COMM_WORLD);
        MPI_Recv(&recv,size,MPI_DOUBLE,get_row_owner(dest),tag,MPI_COMM_WORLD,&stat);
        for(i=0; i<size; i++)
        {
            matrix[src/p][i]=recv[i];
        }
        MPI_Send(vec+src/p,1,MPI_DOUBLE,get_row_owner(dest),tag,MPI_COMM_WORLD);
        MPI_Recv(&t,1,MPI_DOUBLE,get_row_owner(dest),tag,MPI_COMM_WORLD,&stat);
        vec[src/p]=t;
    }
    if(get_row_owner(dest)==rank)
    {
        MPI_Recv(&recv,size,MPI_DOUBLE,get_row_owner(src),tag,MPI_COMM_WORLD,&stat);
        MPI_Send(matrix[dest/p],size,MPI_DOUBLE,get_row_owner(src),tag,MPI_COMM_WORLD);
        for(i=0; i<size; i++)
        {
            matrix[dest/p][i]=recv[i];
        }
        MPI_Recv(&t,1,MPI_DOUBLE,get_row_owner(src),tag,MPI_COMM_WORLD,&stat);
        MPI_Send(vec+dest/p,1,MPI_DOUBLE,get_row_owner(src),tag,MPI_COMM_WORLD);
        vec[dest/p]=t;

    }
}



double* parrarel_gauss_rc(double** matrix, double* vec, int size)
{

    int i,j,rank,p,pivot,tag=0;
    double *vecbuf, bv;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    vecbuf=malloc(sizeof(double)*size);
    for(i=0; i<size; i++)
    {
        pivot = get_global_max(matrix,i%p,i,size);
        MPI_Bcast(&pivot,1,MPI_INT,i%p,MPI_COMM_WORLD);
        exchange_vectors(matrix,vec,i,pivot,size);
        if(rank==i%p)
        {

            memcpy(vecbuf,matrix[i/p],sizeof(double)*size);
            bv=vec[i/p];

        }

        MPI_Bcast(vecbuf,size,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
        MPI_Bcast(&bv,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);

        for(j=(i-rank)/p; j<get_owned_rows(rank,size); j++)
        {
            if(j*p+rank>i)
            {
                double mul=-matrix[j][i]/vecbuf[i];
                add_row(matrix,vecbuf,size,j,mul,i);
                vec[j]+=bv*mul;

            }
        }
    }
    double tres;
    for(i=size-1; i>=0 ; i--)
    {
        if(i%p==rank)
        {
            tres=vec[i/p];
            for(j=size-1; j>i; j--)
            {
                tres-=vecbuf[j]*matrix[i/p][j];
            }
            vecbuf[i]=tres/matrix[i/p][i];
            MPI_Bcast(vecbuf+i,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
        }
        else
        {
            MPI_Bcast(vecbuf+i,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
        }
    }
    return vecbuf;

}



