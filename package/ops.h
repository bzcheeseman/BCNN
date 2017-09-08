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
#include "datatypes.h"

static const int UINT32_SIZE=32;

qarray *quantize(
        float *arr,
        const size_t arr_size,
        const int nbits
) {
  // check that nbits is both a power of 2 and <= 8
  if (!(__builtin_popcountll(nbits) == 1 && nbits <= 8)) {
    return NULL;
  }
  const int q_size = (int)ceilf((float)arr_size/UINT32_SIZE * nbits);
  // the output needs to be array size/32 * number of bits - size==32, bits=1 -> allocate 1 item in array
  qarray *quantized = malloc(sizeof(qarray));
  quantized->data = calloc(q_size, sizeof(uint32_t));
  quantized->size = q_size;
  quantized->nbits = nbits;
  quantized->orig_size = (int)arr_size;
//  printf("q_size: %d\n", q_size);

  uint32_t temp_val = 0;
  float item; int leading_zeros = 0; int q_counter = 0, shift = 0;
  for (int i = 0; i < arr_size; i++) {
    item = arr[i];
    // Get sign bit
    temp_val = ((uint32_t)1-signbit(item)) << nbits-1;

    item = fabsf(item); // already stored sign bit, so get rid of that

    // If nbits == 1 then we're done, skip to else
    if (nbits != 1) {
      // count leading zeros (excluding sign bit)
      leading_zeros = __builtin_clz((uint32_t)item & 0x7fffffff);
      // If we can't fully represent the number with nbits-1, then we gotta shift it over
      if (UINT32_SIZE - leading_zeros >= nbits) {
        temp_val ^= (uint32_t)item >> (UINT32_SIZE-leading_zeros-nbits+1);
      }
      else { // 32-leading zeros < nbits => we can fully represent the number with nbits-1
        temp_val ^= (uint32_t)item; // just put those bits into temp_val
      }
//      printf("%u, %u\n", q_counter, temp_val);
      // Add the bits into the quantized return value now
      quantized->data[q_counter] |= temp_val << shift*nbits; shift++;
      if (shift%(UINT32_SIZE/nbits) == 0 && shift != 0) {
        q_counter++;
        shift = 0;
      }
    }
    else {
      quantized->data[0] |= temp_val << i;
    }
  }

  return quantized;

}

float *dequantize(qarray *arr) {
  float *out = (float *)calloc(arr->orig_size, sizeof(float));

  uint32_t mask = 0;
  int k = 0;
  while (k < arr->nbits) {
    mask |= 1 << k;
    k++;
  }

  uint32_t temp_value = 0; k = 0;
  for (int i = 0; i < arr->size; i++) {
    for (int j = 0; j < UINT32_SIZE ; j+= arr->nbits) {

      if (k >= arr->orig_size) break;

      temp_value = (arr->data[i] & (mask << j)) >> j;
      temp_value ^= 1 << arr->nbits-1; // flip the first bit to standard convention (sign bit)
//      printf("%u ", temp_value);
      if (arr->nbits == 1) {
        out[i * UINT32_SIZE / arr->nbits + j/arr->nbits] = temp_value == 1 ? -1.f : 1.f;
//        printf("%.0f ", out[i * UINT32_SIZE / arr->nbits + j]);
      }
      else {
        out[i * UINT32_SIZE / arr->nbits + j/arr->nbits] = ((float)(temp_value & (mask>>1)) *
                (temp_value >> (arr->nbits-1) == 1 ? -1.f : 1.f));
//        printf("%f ", out[i * UINT32_SIZE / arr->nbits + j/arr->nbits]);
      }

      k++;

    }
  }
  return out;
}

#endif //BCNN_OPS_H
