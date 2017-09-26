#include <stdio.h>
#include "package/quantize_ops.h"

int main() {
  size_t arr_size = 21;
  float *arr = calloc(arr_size, sizeof(float));
  arr[0] = 1.01235f;
  arr[1] = -1.14f;
  arr[2] = -0.2f;
  qtensor *quantized = quantize(arr, arr_size, 8);
  print_qtensor(quantized);
  arr = dequantize(quantized);
  printf("%f ", arr[0]);
  printf("%f ", arr[1]);
  printf("%f ", arr[2]);
  printf("\n");
  float *arr_window = get_arr_window(&arr, &arr_size, 2);
//  printf("%u ", arr_size);
  printf("%f ", arr_window[0]);
  printf("%f ", arr_window[1]);
  printf("%f ", arr[0]);
//  printf("%f ", arr[1]);
//  printf("%f ", arr[2]);
  return 0;
}