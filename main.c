#include <stdio.h>
#include "package/quantize_ops.h"

int main() {
  int arr_size = 21;
  float *arr = calloc(arr_size, sizeof(float));
  arr[0] = 3.21235f;
  arr[1] = -1.14f;
  arr[2] = -5.2f;
  qarray *quantized = quantize(arr, arr_size, 8); // nbits=8 works, nbits=4 works, nbits=2 works, nbits=1 at all (even) array sizes
  print_qarray(quantized);
  dequantize(quantized);
  return 0;
}