#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <math.h>

#define ICEIL iceil(sizes[0],p)

int iceil(int num, int div)
{
    float fnum = num;
    float fdiv = div;
    return ceil(fnum/fdiv);
}

int main(int argc, char* argv[])
{

    int rank,p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int sizes[2];
    float **matrix,*vec,*resvec;
    int i,j,*t,root=0;
    t=malloc(16);
    if(rank==root)
    {
        srand(0);
        /** here goes matrix and vec input **/
        sizes[0]=5;//=rand()%5000;
        sizes[1]=5;//=rand()%5000;

        matrix = malloc(sizeof(float*)*(sizes[0]+p));
        vec = malloc(sizeof(float)*(sizes[1]));
        resvec = calloc(sizes[1]+p,sizeof(float));
        for(i=0; i<sizes[1]; i++)
            vec[i]=i;
        for(i=0; i<sizes[0]+p; i++)
        {

            matrix[i]=malloc(sizeof(float)*sizes[1]);
            if(i<sizes[0])
                for(j=0; j<sizes[1]; j++)
                    matrix[i][j]=i*sizes[0]+j;//rand()%10000+i+j;
        }
    }

    /** Broadcast size of matrix (m * n) and vector (m) **/

    MPI_Bcast(sizes,2,MPI_INT,root,MPI_COMM_WORLD);
    MPI_Request req[p];
    MPI_Status stat;
    /** Alloc matrix and  vec parts **/
    if(rank!=root)
    {

        matrix=malloc(sizeof(float*)*iceil(sizes[0],p));
        vec=malloc(sizeof(float)*sizes[1]);
        resvec = calloc(ICEIL,sizeof(float));
        int bar = iceil(sizes[0],p);
        for(i=0; i<bar; i++)
        {
            matrix[i]=malloc(sizeof(float)*sizes[1]);
        }

    }
    /** Scatter vec and matrix **/

    MPI_Request sreq;

    MPI_Bcast(vec,sizes[1],MPI_FLOAT,root,MPI_COMM_WORLD);
    int icl = ICEIL;

    if(rank==root)
    {


        for(i=0; i<p-1; i++)
        {
            for(j=0; j<icl; j++)
                MPI_Isend(matrix[(i+1)*icl+j],sizes[1],MPI_FLOAT,i+1,0,MPI_COMM_WORLD,&sreq);
        }

    }
    else
    {
        for(i=0; i<icl; i++)
            MPI_Recv(matrix[i],sizes[1],MPI_FLOAT,root,0,MPI_COMM_WORLD,&stat);
    }
    /** multiplication **/
    for(i=0; i<ICEIL; i++)
    {
        for(j=0; j<sizes[1]; j++)
            resvec[i]+=matrix[i][j]*vec[j];
    }

    /** Gather partial vec multiplication **/
    if(rank==root)
    {
        for(i=1; i<p; i++)
            MPI_Irecv(resvec+i*iceil(sizes[0],p),iceil(sizes[0],p),MPI_FLOAT,i,0,MPI_COMM_WORLD,&req[i]);

        for(i=1; i<p; i++)
            MPI_Wait(&req[i],&stat);

    }
    /** If not root send your part **/
    else
    {

        MPI_Send(resvec,iceil(sizes[0],p),MPI_FLOAT,root,0,MPI_COMM_WORLD);
    }



    if(rank==root)
    {
        for(i=0; i<sizes[1]; i++)
        {
            printf("%f \n",resvec[i]);
        }
    }
    /*{

        for(i=0; i<sizes[0]; i++)
        {
            for(j=0; j<sizes[1]; j++)
                printf("%f ", matrix[i][j]);
            printf(" %f\n",vec[i]);
        }
        MPI_Send(t,1,MPI_INT,root+1,0,MPI_COMM_WORLD);

    }
    else
    {

        int bar = ICEIL;
        MPI_Recv(t,1,MPI_INT,rank-1,0,MPI_COMM_WORLD,&stat);
        printf("\n");
        for(i=0; i<bar; i++)
        {

            for(j=0; j<sizes[1]; j++)
                printf("%f ", matrix[i][j]);
            printf(" %f",vec[i]);
            printf("\n");
        }
        if(rank!=p-1)
            MPI_Send(t,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);

    }*/
    MPI_Finalize();


    return 0;
}
