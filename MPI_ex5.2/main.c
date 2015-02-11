#include <stdio.h>
#include <stdlib.h>
#include "ringgather.h"

#include "mpi.h"



int main(int argc, char* argv[])
{
    int rank, p;
    MPI_Init(&argc,&argv);
    float *x,*y;
    int *sizes, *offsets, mysize;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);


    sizes=malloc(sizeof(int)*p);
    offsets=malloc(sizeof(int)*p);

    mysize=rank+1;

    Gather_ring_sizes(sizes,mysize);
    Sizes_count_offsets(offsets,sizes,p);

    x=malloc(sizeof(float)*mysize);
    *x=rank;
    y=malloc(sizeof(float)*(offsets[p-1]+sizes[p-1]));

    Gatherv_ring_nb(y,sizes,offsets,x);

    int i,j;
    if(rank==0)
    for(i=0; i<p; i++)
    {
        printf("Data from p=%i\n",i);
        for(j=0; j<sizes[i]; j++)
        {
            printf("%f ",y[offsets[i]+j]);
        }
        printf("\n");

    }
    /** CODE GOES HERE **/

    MPI_Finalize();
    return 0;
}
