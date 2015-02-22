#include <stdio.h>
#include <stdlib.h>

#include "local_matrix.h"
#include <math.h>
#include <time.h>


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
typedef struct TDM
{
    double* a;
    double* b;
    double* c;
    double* y;
    int size;
} TDM;

TDM* allocate_tdm(int size);
void randomize_tdm(TDM* tdm, int seed);
inline double tdm_geta(int k, TDM* tdm);
inline double tdm_getb(int k, TDM* tdm);
inline double tdm_getc(int k, TDM* tdm);
inline double tdm_gety(int k, TDM* tdm);
inline double a_getx(double* a, int size, int k);
void tdm_display(TDM* tdm);


TDM* reduction(TDM* tdm, int step)
{
    TDM* ttdm = allocate_tdm(tdm->size);
    int stepsize=1;
    int i,j;

    for(i=0; i<step; i++) stepsize*=2;
    for(i=stepsize-1; i<tdm->size; i+=stepsize)
    {
        double alpha,beta;
        double a1,b1,c1,a2,b2,c2,y1,y2;
        a1=tdm_geta(i-stepsize,tdm);
        b1=tdm_getb(i-stepsize,tdm);
        c1=tdm_getc(i-stepsize,tdm);
        y1=tdm_gety(i-stepsize,tdm);
        a2=tdm_geta(i+stepsize,tdm);
        b2=tdm_getb(i+stepsize,tdm);
        c2=tdm_getc(i+stepsize,tdm);
        y2=tdm_gety(i+stepsize,tdm);
        alpha=-tdm_geta(i,tdm)/b1;
        beta=-tdm_getc(i,tdm)/b2;
        if(b1==0) alpha=0;
        if(b2==0) beta=0;
        ttdm->y[i]=tdm_gety(i,tdm)+alpha*y1+beta*y2;
        ttdm->b[i]=tdm_getb(i,tdm)+alpha*c1+beta*a2;
        if(i-stepsize-stepsize>=0) ttdm->a[i]=alpha*a1;
        if(i+stepsize+stepsize<tdm->size) ttdm->c[i]=beta*c2;


    }
    return ttdm;
}
double** matrix;
double* vec;
int main()
{



    int z,i,j,k=1,stepsize=1;
    scanf("%i",&i);
    TDM* tdms[i];
    z=i;
    while(i--) k*=2;
    TDM* tdm = allocate_tdm(k);
    tdms[0]=tdm;
    randomize_tdm(tdm,0);
    tdm_display(tdm);
    double resvec[k];
    for(i=0; i<k; i++) resvec[i]=0;
    for(i=0; i<z; i++)
    {

            tdms[i+1]=reduction(tdms[i],i);
            tdm_display(tdms[i+1]);

    }
    i=z+1;
    while(i--)
    {
        stepsize*=2;
    }
    for(i=z; i>=0; i--)
    {
        for(j=stepsize/2-1; j<k; j+=stepsize)
        {
            resvec[j]=(tdm_gety(j,tdms[i])-tdm_geta(j,tdms[i])*a_getx(resvec,k,j-stepsize/2)-tdm_getc(j,tdms[i])*a_getx(resvec,k,j+stepsize/2))/tdm_getb(j,tdms[i]);
        }
        for(j=0; j<k; j++)
            printf("x%i=%f\n",j,resvec[j]);
        stepsize/=2;
    }

    return 0;
}
TDM* allocate_tdm(int size)
{
    TDM* ret = malloc(sizeof(TDM));
    ret->a=calloc(sizeof(double),size);
    ret->b=calloc(sizeof(double),size);
    ret->c=calloc(sizeof(double),size);
    ret->y=calloc(sizeof(double),size);
    ret->size=size;
    return ret;
}

double tdm_geta(int k, TDM* tdm)
{
    if(!tdm) return NAN;
    if(k>0&&tdm->size>k)
        return tdm->a[k];
    return 0;
}

double tdm_getb(int k, TDM* tdm)
{
    if(!tdm) return NAN;
    if(k>=0&&tdm->size>k)
        return tdm->b[k];
    return 1;
}

double tdm_getc(int k, TDM* tdm)
{
    if(!tdm) return NAN;
    if(k>=0&&tdm->size>k-1)
        return tdm->c[k];
    return 0;
}

double tdm_gety(int k, TDM* tdm)
{
    if(!tdm) return NAN;
    if(k>=0&&tdm->size>k)
        return tdm->y[k];
    return 0;
}
void tdm_display(TDM* tdm)
{
    if(!tdm) return;
    printf("/=/ \t\t /=/\n");
    int i,j;
    for(i=0; i<tdm->size; i++)
    {
        for(j=0; j<i-1; j++)
            printf("%f ",0.f);
        if(i-1>=0) printf("%f ",tdm_geta(i,tdm));
        printf("%f ",tdm_getb(i,tdm));
        if((i+1)<(tdm->size)) printf("%f ",tdm_getc(i,tdm));
        for(j=i+2; j<tdm->size; j++)
            printf("%f ",0.f);
        printf(" = %f",tdm->y[i]);
        printf("\n");
    }
    printf("/=/ \t\t /=/\n");
}
void randomize_tdm(TDM* tdm, int seed)
{
    srand(seed);
    int i;
    for(i=0; i<tdm->size; i++)
        tdm->b[i]=rand()%1000-500;
    for(i=0; i<tdm->size; i++)
        tdm->y[i]=rand()%1000-500;
    for(i=1; i<tdm->size; i++)
        tdm->a[i]=rand()%500-250;
    for(i=0; i<tdm->size-1; i++)
        tdm->c[i]=rand()%500-250;
}
double a_getx(double* a, int size, int k)
{
    if(!a) return NAN;
    if(k>size||k<0) return 0;
    return a[k];
}
