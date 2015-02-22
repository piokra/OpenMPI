#include <stdio.h>
#include <stdlib.h>

#include "local_matrix.h"
#include <math.h>
#include <time.h>


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

void reduction(double** matrix, double* vector, int size, int step)
{
    double tmatrix[size][size], tvec[size];
    int stepsize=1;
    int i,j;
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            tmatrix[i][j]=0;
        }
    }
    for(i=0; i<step; i++) stepsize*=2;
    for(i=0; i<size; i++)
    {
        double alpha,beta;
        double a1,b1,c1,a2,b2,c2,y1,y2;
        if(i-stepsize>=0)
        {
            if(i-stepsize-stepsize>=0)
            {
                a1=matrix[i-stepsize][i-stepsize-stepsize];
            }
            else
            {
                a1=0;
            }
            b1=matrix[i-stepsize][i-stepsize];
            c1=matrix[i-stepsize][i];
            y1=vector[i-stepsize];
            alpha=-matrix[i][i-stepsize]/b1;

        }
        else
        {
            a1=0;
            b1=1;
            c1=0;
            alpha=0;
            y1=0;
        }
        if(i+stepsize<size)
        {
            if(i+stepsize+stepsize<size)
            {
                c2=matrix[i+stepsize][i+stepsize+stepsize];
            }
            else
            {
                c2=0;
            }
            y2=vector[i+stepsize];
            b2=matrix[i+stepsize][i+stepsize];
            a2=matrix[i+stepsize][i];
            beta=-matrix[i][i+stepsize]/b2;
        }
        else
        {
            a2=0;
            b2=1;
            c2=0;
            beta=0;
            y2=0;
        }
        tvec[i]=vector[i]+alpha*y1+beta*y2;
        tmatrix[i][i]=matrix[i][i]+alpha*c1+beta*a2;
        if(i-stepsize-stepsize>=0) tmatrix[i][i-stepsize-stepsize]=alpha*a1;
        if(i+stepsize+stepsize<size) tmatrix[i][i+stepsize+stepsize]=beta*c2;


    }
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            matrix[i][j]=tmatrix[i][j];
        }
        vector[i]=tvec[i];
    }
}
double** matrix;
double* vec;
int main()
{
    srand(time(NULL));
    int z,i,j,k=1;
    scanf("%i",&i);
    z=i;
    while(i--) k*=2;
    matrix=allocate_matrix(k,k);
    vec=malloc(sizeof(double)*k);
    for(i=0; i<k; i++)
    {
        vec[i]=rand()%100;
        matrix[i][MAX(0,i-1)]=rand()%1000-500;
        matrix[i][i]=rand()%2000-1000;
        matrix[i][MIN(k-1,i+1)]=rand()%1000-500;
    }
    display_matrix(matrix,k,k);
    for(i=0; i<z; i++)
    {

            reduction(matrix,vec,k,i);
            display_matrix(matrix,k,k);
            for(j=0; j<k; j++)
                printf("vec=%lf\n",vec[j]);
    }
    for(i=0; i<k; i++)
        printf("res=%lf\n",vec[i]/matrix[i][i]);

    return 0;
}
