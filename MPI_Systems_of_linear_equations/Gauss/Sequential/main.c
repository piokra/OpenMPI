#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "local_matrix.h"

int main(int argc, char* argv[])
{

    int i,rank,p,size=4;
    double seq_s, seq_e;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    double** matrix = 0;
    double* vec = 0 ;
    double* rvec = 0;
    if(rank)
    {
        MPI_Finalize();
        return -1;
    }
    if(!rank)
    {
        load_vector("vec.dat",&vec,&size);
        load_matrix("matrix.dat",&matrix, &size, &size);

    }
    seq_s=MPI_Wtime();
    rvec = Seq_gauss(matrix,vec,size,size);



    seq_e=MPI_Wtime();
    printf("Sequential gauss work time=%f\n",seq_e-seq_s);
    double res=0;

    if(!rank)
    {


        save_vector("result.dat",rvec,size);
    }
    MPI_Finalize();

    return 0;
}
