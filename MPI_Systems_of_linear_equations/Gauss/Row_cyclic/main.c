#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "local_matrix.h"
/** MATRIX INDEXES
    (y,x)

    0,0 0,1 0,2 ...
    1,0 1,1 ...
    2,0 ...
    ...

                    **/


int main(int argc, char* argv[])
{

    int rank,p,size=1000;
    double seq_s,seq_e;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    /** Code goes here **/
    double* vec = malloc(sizeof(double)*size);

    double** matrix = allocate_matrix(size,size);

    randomize_matrix(matrix,size,size,0,1000,-1000);
    //display_matrix(matrix,size,size);
    seq_s=MPI_Wtime();
    Seq_gauss(matrix,vec,size,size);
    seq_e=MPI_Wtime();
    printf("Upper traingular\n");
    //display_matrix(matrix,size,size);
    printf("Seq gauss MPI time=%f\n",seq_e-seq_s);


    MPI_Finalize();

    return 0;



}
