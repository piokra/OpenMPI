#ifndef LOCAL_MATRIX_H
#define LOCAL_MATRIX_H

#include "mpi.h"

double** allocate_matrix(int y, int x);
void randomize_matrix(double** matrix, int y, int x, int seed, double min, double max);
void save_matrix(const char* str, double** matrix, int x, int y);
void load_matrix(const char* str, double*** matrix, int* x, int* y);
void save_vector(const char* str, double* vec, int size);
void load_vector(const char* str, double** vec, int* size);
void display_matrix(double** matrix, int y, int x);
void free_matrix(double** matrix, int x, int y);
#endif // LOCAL_MATRIX_H

