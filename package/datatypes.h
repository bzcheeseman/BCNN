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

typedef struct quantized_array qarray;

void print_qarray(qarray *arr) {
  printf("qarray nbits: %d\n", arr->nbits);
  printf("qarray individual elements: %d\n", arr->nelements);
  for (int i = 0; i < arr->nbits; i++) {
    printf("%u ", arr->data[i]);
  }
  printf("\n");
}

#endif //BCNN_DATATYPES_H
