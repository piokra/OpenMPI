#include "local_matrix.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
double** allocate_matrix(int x, int y)
{
    int i;
    double** temp = malloc(sizeof(double*)*y);

    for(i=0; i<y; i++)
    {
        temp[i]=malloc(sizeof(double)*x);
    }
    return temp;
}

void randomize_matrix(double** matrix, int x, int y, int seed, double max, double min)
{
    srand(seed);
    int i,j;
    for(i=0; i<x; i++)
        for(j=0; j<y; j++)
            matrix[i][j]=rand()%((int)ceil(max-min))-ceil(min);
}


void save_matrix(const char* str, double** matrix, int x, int y)
{

}

void load_matrix(const char* str, double** matrix, int x, int y)
{

}
void display_matrix(double** matrix, int x, int y)
{
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}
void free_matrix(double** matrix, int x, int y)
{
    int i;
    for(i=0; i<x; i++)
    {
        free(matrix[x]);
    }
    free(matrix);
}
