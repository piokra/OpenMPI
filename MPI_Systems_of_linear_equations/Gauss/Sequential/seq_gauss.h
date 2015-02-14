#ifndef SEQ_GAUSS_H
#define SEQ_GAUSS_H

#define ABS(a) (((a)<0)?(-(a)):((a)))
void Seq_add_row(double** matrix, double* vec, int src, int dest, int size, double mul,int k);
void Seq_pivot(double** matrix, int k, int xm, int ym);
double* Seq_gauss(double** matrix, double* vec, int x, int y);

#endif // SEQ_GAUSS_H

