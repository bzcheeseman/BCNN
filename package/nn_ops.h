//
// Created by Aman LaChapelle on 9/7/17.
//
// BCNN
// Copyright (c) 2017 Aman LaChapelle
// Full license at BCNN/LICENSE.txt
//

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef BCNN_BLAS_OPS_H
#define BCNN_BLAS_OPS_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "datatypes.h"

// access individual bits with bitmask and nbits stride
// void gemm; // xnor-style gemm using the arbitrary quantization in quantize_ops!

// xnor-style convolutions done in a smart way with bit shifting
// out should really be a qtensor, which means we need to take the output and re-binarize it
void convolve(qtensor *data, qtensor *kernel, int stride, qtensor *out) {
  assert(kernel->nbits == data->nbits);

  int nbits = kernel->nbits;
  int kernel_size = kernel->nelements;
  uint32_t mask = (1 << kernel_size+1) - 1;

  const int output_size = (data->nelements - kernel_size)/stride + 1;

  out->nbits = nbits;
  out->nelements = output_size;

  uint32_t temp = 0;
  for (int i = 0; i < output_size; i++) {
    temp = 0;
    for (int b = 0; b < nbits; b++) { // collapses over the number of bits...will need to binarize again...
      temp += __builtin_popcount(~((kernel->data[b] << i*stride) ^ data->data[b]) & (mask << i*stride));
//      out->data[b] = 0; // setup for re-binarization
    }
    printf("%u ", temp);
    for (int b = 0; b < nbits; b++) {
      out->data[b] |= ((temp & (1 << nbits-b)) >> nbits-b) << i;
    }
  }
}


#endif //BCNN_BLAS_OPS_H
