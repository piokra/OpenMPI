#ifndef RINGGATHER_H
#define RINGGATHER_H

#include "mpi.h"
#include <stdlib.h>
void Gather_ring_nb(float* y, int blocksize, float* x);
void Gather_ring_sizes(int* sizes, int mysize);
void Gatherv_ring_nb(float* y, int* sizes,int* offsets, float* x);

void Sizes_count_offsets(int* res, int* sizes, int count);

#endif // RINGGATHER_H

