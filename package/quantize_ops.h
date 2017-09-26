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


#ifndef BCNN_OPS_H
#define BCNN_OPS_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "datatypes.h"

float *get_arr_window( // need to test this code
        float **arr, // pointer to first element of array (gets changed in here)
        size_t *arr_size, // size of array (gets changed in here)
        const size_t window_size
) {
  if (*arr_size - window_size < 0 || window_size > UINT32_SIZE) {
    return NULL;
  }

  float *out = calloc(window_size, sizeof(float));
  memcpy(out, *arr, window_size*sizeof(float));
  if (*arr_size - window_size > 0) {
    *arr += window_size; // advance pointer by window_size
    *arr_size -= window_size;
  }
  else if (*arr_size - window_size == 0) {
    *arr = NULL;
    *arr_size = 0;
  }

  return out;

}

qtensor *quantize(
        float *arr,
        const size_t arr_size,
        const int nbits
) {
  // check that nbits is both a power of 2 and <= 8
  if (!(__builtin_popcountll(nbits) == 1 && nbits <= 8)) {
    return NULL;
  }
  if (arr_size > 32) { // we want to do this in windows if the array is larger than 32
    return NULL;
  }

  const int q_size = nbits; // nth item holds nth bit

  qtensor *quantized = malloc(sizeof(qtensor));
  quantized->data = calloc((size_t)q_size, sizeof(uint32_t));
  quantized->nbits = nbits;
  quantized->nelements = (int)arr_size;

  float item; uint32_t temp_val = 0;
  for (int i = 0; i < arr_size; i++) {
    item = arr[i];

    quantized->data[0] |= ((uint32_t)1-signbit(item)) << i;
    if (nbits != 1) {
      item = fabsf(item);
      item *= powf(2, nbits);

      // everything but the sign bit
      temp_val = (uint32_t)item & 0x7fffffff;

      // temp_val now holds the nbit representation of this item of the array (no sign bit)
      for (int b = 0; b < nbits-1; b++) {
        quantized->data[b+1] |= ((temp_val & (1 << nbits-b)) >> nbits-b) << i;
      }
    }
  }

  return quantized;

}

float *dequantize(
        qtensor *arr
) {
  float *out = (float *)calloc((size_t)arr->nelements, sizeof(float));

  uint32_t temp_val = 0;
  for (int i = 0; i < arr->nelements; i++) {
    int sign = 1-((arr->data[0] & (1 << i)) >> i); // set sign bit
    if (arr->nbits == 1) {
      out[i] = 1 - 2*sign;
    }
    else {
      temp_val = 0;
      for (int b = 0; b < arr->nbits-1; b++) {
        temp_val |= ((arr->data[b+1] & (1 << i)) >> i) << arr->nbits-b;
      }
      out[i] = (float)temp_val * powf(2, -arr->nbits);
      if (sign) {
        out[i] *= -1;
      }
    }
  }

  return out;
}

#endif //BCNN_OPS_H
