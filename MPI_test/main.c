/*
 * Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 *
 * Sample MPI "hello world" application in C
 */

#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char* argv[])
{

    double ain[30], aout[30];
    struct{double val; int rank;} in[30], out[30];
    int i,my_rank,root=0;

    char version[MPI_MAX_LIBRARY_VERSION_STRING];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    srand(my_rank);
    for(i=0; i<30; i++)
    {
        ain[i]=rand()%15023;

    }

    for(i=0; i<30; i++)
    {
        in[i].val=ain[30];
        in[i].rank=my_rank;
    }
    MPI_Reduce(in,out,30,MPI_DOUBLE_INT,MPI_MAXLOC,root,MPI_COMM_WORLD);
    if(my_rank==root)
    {
        for(i=0; i<30; i++)
        {
            printf("Rank=%i val=%f\n",out[i].rank,out[i].val);
        }
    }

    MPI_Finalize();

    return 0;
}
