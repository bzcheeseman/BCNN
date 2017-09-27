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


#ifndef BCNN_DATATYPES_H
#define BCNN_DATATYPES_H

#include <stdint.h>
#include <stdio.h>

const int UINT32_SIZE=32;

struct quantized_array {
  uint32_t *data;
  int nelements;
  int nbits; // also stride!
};

typedef struct quantized_array qtensor;

void print_qtensor(qtensor *arr) {
  printf("qarray nbits: %d\n", arr->nbits);
  printf("qarray individual elements: %d\n", arr->nelements);
  for (int b = 0; b < arr->nbits; b++) {
    printf("%u ", arr->data[b]);
  }
  printf("\n");
}

struct tensor_t {
  float *data; // [i, j, k, l] = [i*c*h*w + j*h*w + k*w + l] = [((i*c + j)*h + k)*w + l]
  int stride[3];
  int dims[4]; // n, c, h, w
};

typedef struct tensor_t tensor;

tensor *new_tensor(float *data, int n, int c, int h, int w) { // takes ownership of ptr
  tensor *out = malloc(sizeof(tensor));
  out->data = data;
  out->dims[0] = n;
  out->dims[1] = c;
  out->dims[2] = h;
  out->dims[3] = w;
  out->stride[0] = c*h*w;
  out->stride[1] = h*w;
  out->stride[2] = w;

  // tensor(i, j, k, l) = data[((i*stride[0] + j)*stride[1] + k)*stride[2] + l]
}

tensor *slice(tensor *in, int axis, int n_elements) {
  tensor *out = malloc(sizeof(tensor));
  // gotta get this to work...
  return out;
}

#endif //BCNN_DATATYPES_H
