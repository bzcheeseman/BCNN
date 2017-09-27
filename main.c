#include <stdio.h>
#include "package/quantize_ops.h"
#include "package/nn_ops.h"

int main() {
  int nbits = 8;

  size_t arr_size = 21;
  float *arr = calloc(arr_size, sizeof(float));
  arr[0] = 1.21235f;
  arr[1] = -1.14f;
  arr[2] = -1.2f;

  qtensor *quantized = quantize(arr, arr_size, nbits);
  print_qtensor(quantized);
  arr = dequantize(quantized);
  printf("%f ", arr[0]);
  printf("%f ", arr[1]);
  printf("%f ", arr[2]);
  printf("\n");

  size_t kernel_size = 3;
  float *kernel = calloc(kernel_size, sizeof(float));
  kernel[0] = 1.212f;
  kernel[1] = -1.02f;
  kernel[2] = -1.0f;

  qtensor *qkern = quantize(kernel, 3, nbits);
//  kernel = dequantize(qkern);
//  printf("%f ", kernel[0]);
//  printf("%f ", kernel[1]);
//  printf("%f ", kernel[2]);
//  printf("\n");

  int stride = 1;

  int output_size = (arr_size - kernel_size)/stride + 1;
  qtensor *out = malloc(sizeof(qtensor));
  out->data = calloc(nbits, sizeof(uint32_t));
  convolve(quantized, qkern, stride, out);
  print_qtensor(out);
//  float *arr_window = get_arr_window(&arr, &arr_size, 2);
//  printf("%u ", arr_size);
//  printf("%f ", arr_window[0]);
//  printf("%f ", arr_window[1]);
//  printf("%f ", arr[0]);
//  printf("%f ", arr[1]);
//  printf("%f ", arr[2]);
  return 0;
}