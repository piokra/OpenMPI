#include "local_matrix.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
double** allocate_matrix(int y, int x)
{
    int i;
    double** temp = malloc(sizeof(double*)*y);

    for(i=0; i<y; i++)
    {
        temp[i]=calloc(sizeof(double),x);
    }
    return temp;
}

void randomize_matrix(double** matrix, int y, int x, int seed, double max, double min)
{
    srand(seed);
    int i,j;
    for(i=0; i<y; i++)
        for(j=0; j<x; j++)
            matrix[i][j]=rand()%((int)ceil(max-min))-ceil(min);
}


void save_matrix(const char* str, double** matrix, int x, int y)
{
    FILE* file = 0;
    file = fopen(str,"w+");
    fprintf(file,"%i %i\n",x,y);
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            fprintf(file,"%.12lf ",matrix[i][j]);
        }
        fprintf(file,"\n");
    }
    fclose(file);

}

void load_matrix(const char* str, double*** matrix, int* x, int* y)
{
    FILE* file = fopen(str,"r");
    int tx, ty;
    double** tmatrix;
    fscanf(file,"%i%i",&tx,&ty);
    int i,j;

    tmatrix=malloc(sizeof(double)*ty);
    for(i=0; i<ty; i++)
    {
        tmatrix[i]=malloc(sizeof(double)*tx);
        for(j=0; j<tx; j++)
        {
            fscanf(file,"%lf",&(tmatrix[i][j]));

        }
    }
    *x=tx; *y=ty;
    *matrix=tmatrix;
    fclose(file);
}
void save_vector(const char* str, double* vec, int size)
{
    FILE* file = 0;
    file = fopen(str,"w");
    int i;
    fprintf(file,"%i\n",size);
    for(i=0; i<size; i++)
    {
        fprintf(file,"%.12lf ",vec[i]);
    }
    fprintf(file,"\n");
    fclose(file);
}
void load_vector(const char* str, double** vec, int* size)
{
    FILE* file = 0;
    file = fopen(str,"r");
    int i,j;
    fscanf(file,"%i",&i);
    double* tvec = malloc(sizeof(double)*i);
    for(j=0; j<i; j++)
    {
        fscanf(file,"%lf",&(tvec[j]));

    }
    *size=i;
    *vec=tvec;
    fclose(file);
}
void display_matrix(double** matrix, int y, int x)
{
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            printf("(%i,%i)%f ", i,j,matrix[i][j]);
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
