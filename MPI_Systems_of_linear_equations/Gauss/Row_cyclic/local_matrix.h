#ifndef LOCAL_MATRIX_H
#define LOCAL_MATRIX_H

#include "mpi.h"

double** allocate_matrix(int x, int y);
void randomize_matrix(double** matrix, int x, int y, int seed, double min, double max);
void save_matrix(const char* str, double** matrix, int x, int y);
void load_matrix(const char* str, double** matrix, int x, int y);
void display_matrix(double** matrix, int x, int y);
void free_matrix(double** matrix, int x, int y);
#endif // LOCAL_MATRIX_H

