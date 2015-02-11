#include "seq_gauss.h"
#include <stdlib.h>
#include <stdio.h>
#include "local_matrix.h"

#define ABS(a) (((a)<0)?(-(a)):((a)))
void Seq_add_row(double** matrix, double* vec, int src, int dest, int size, double mul)
{
    int i;
    vec[dest]+=vec[src]*mul;
    for(i=0; i<size; i++)
    {
        matrix[dest][i]+=matrix[src][i]*mul;
    }
}
void Seq_pivot(double** matrix, int k, int xm, int ym)
{
    int i,max_col; double max_val=0 ,t=0;
    for(i=k; i<xm; i++)
    {
        if(max_val<matrix[k][i])
        {
            max_col=i;
            max_val=matrix[k][i];
        }
    }
    for(i=0; i<ym; i++)
    {
        t=matrix[i][k];
        matrix[i][k]=matrix[i][max_col];
        matrix[i][max_col]=t;
    }
}
double* Seq_gauss(double** matrix, double* vec, int x, int y)
{
    double* resvec = malloc(sizeof(double)*x);
    int i,j,k;
    double mul;
    for(i=0; i<x; i++)
    {
        Seq_pivot(matrix,i,x,y);
        for(j=i+1; j<x; j++)
        {
            mul=-matrix[j][i]/matrix[i][i];
            Seq_add_row(matrix,vec,i,j,x,mul);
        }
    }
    double tres;
    for(i=x-1; i>=0; i--)
    {
        tres=vec[i];
        for(j=x-1; j<i; j--)
        {
            tres-=resvec[j]/matrix[i][j];
        }
        resvec[i]=tres/matrix[i][i];
    }
    return resvec;
}
