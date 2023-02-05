#include <stdio.h>

typedef union {
  struct {
    unsigned fraction:6;
    unsigned exponent:5;
    unsigned sign:1;
  } parts;
  unsigned short value;
} float12;

// Convert a 32-bit float to a 12-bit float
float12 float_to_float12(float f) {
  float12 result;
  result.parts.sign = (f < 0);
  result.parts.exponent = (unsigned char)(fabs(f) / 2);
  result.parts.fraction = (unsigned char)(fabs(f) * 64) & 63;
  return result;
}

// Convert a 12-bit float to a 32-bit float
float float12_to_float(float12 f12) {
  return (f12.parts.sign ? -1 : 1) * (f12.parts.fraction / 64.0 + f12.parts.exponent * 2);
}

// Add two 12-bit floats
float12 float12_add(float12 a, float12 b) {
  return float_to_float12(float12_to_float(a) + float12_to_float(b));
}

// Subtract two 12-bit floats
float12 float12_sub(float12 a, float12 b) {
  return float_to_float12(float12_to_float(a) - float12_to_float(b));
}

// Multiply two 12-bit floats
float12 float12_mul(float12 a, float12 b) {
  return float_to_float12(float12_to_float(a) * float12_to_float(b));
}

// Divide two 12-bit floats
float12 float12_div(float12 a, float12 b) {
  return float_to_float12(float12_to_float(a) / float12_to_float(b));
}

// Convolutional Layer 1
void conv_layer1(float12 input[], float12 weights[], float12 biases[], float12 output[], int input_size, int filter_size) {
  int i, j, k, l;
  float12 sum;

  for (i = 0; i < input_size - filter_size + 1; i++) {
    for (j = 0; j < input_size - filter_size + 1; j++) {
      for (k = 0; k < filter_size; k++) {
        for (l = 0; l < filter_size; l++) {
          sum = float12_add(sum, float12_mul(input[(i+k)*input_size + (j+l)], weights[k*filter_size + l]));
        }
      }
      output[i*(input_size-filter_size+1) + j] = float12_add(sum, biases[0]);
      sum = float_to_float12(0.0);
    }
  }
}

// Convolutional Layer 2
void conv_layer2 (float12 *input, float12 *output, float12 *weights, float12 *bias, int input_height, int input_width, int output_height, int output_width, int kernel_size, int stride, int padding) {
int filter_height = kernel_size;
int filter_width = kernel_size;
int output_idx = 0;
int filter_idx = 0;
int input_idx = 0;

for (int out_y = 0; out_y < output_height; out_y++) {
for (int out_x = 0; out_x < output_width; out_x++) {
float12 result = {0};
for (int filter_y = 0; filter_y < filter_height; filter_y++) {
for (int filter_x = 0; filter_x < filter_width; filter_x++) {
int in_y = out_y * stride + filter_y - padding;
int in_x = out_x * stride + filter_x - padding;
if (in_y >= 0 && in_y < input_height && in_x >= 0 && in_x < input_width) {
input_idx = in_y * input_width + in_x;
result = float12_add(result, float12_mul(weights[filter_idx], input[input_idx]));
}
filter_idx++;
}
}
output[output_idx++] = float12_add(result, bias[out_y * output_width + out_x]);
}
}
}

void max_pooling_layer2(float12 *input, float12 *output, int input_height, int input_width, int output_height, int output_width, int kernel_size, int stride) {
int output_idx = 0;
for (int out_y = 0; out_y < output_height; out_y++) {
for (int out_x = 0; out_x < output_width; out_x++) {
float12 max_val = {0};
for (int pool_y = 0; pool_y < kernel_size; pool_y++) {
for (int pool_x = 0; pool_x < kernel_size; pool_x++) {
int in_y = out_y * stride + pool_y;
int in_x = out_x * stride + pool_x;
int input_idx = in_y * input_width + in_x;
max_val = (input[input_idx].value > max_val.value) ? input[input_idx] : max_val;
}
}
output[output_idx++] = max_val;
}
}
}
float12 pooling(float12 input[][2], int row, int col) {
  float12 max = input[0][0];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (input[i][j].value > max.value) {
        max = input[i][j];
      }
    }
  }
  return max;
}


float12 tanh(float12 input) {
  float x = (input.parts.sign ? -1 : 1) * (input.parts.fraction / 64.0 + input.parts.exponent * 2);
  return float_to_float12(tanh(x));
}

// Convert a 32-bit float to a 12-bit float
float12 float_to_float12(float f) {
  float12 result;
  result.parts.sign = (f < 0);
  result.parts.exponent = (unsigned char)(fabs(f) / 2);
  result.parts.fraction = (unsigned char)(fabs(f) * 64) & 63;
  return result;
}

// Convert a 12-bit float to a 32-bit float
float float12_to_float(float12 f12) {
  return (f12.parts.sign ? -1 : 1) * (f12.parts.fraction / 64.0 + f12.parts.exponent * 2);
}

int main(void) {
  float12 input[2][2] = { { float_to_float12(0.5), float_to_float12(-0.5) },
                         { float_to_float12(-0.5), float_to_float12(0.5) } };
  float12 pool = pooling(input, 2, 2);
  float12 tanh_output = tanh(pool);
  printf("Tanh output: %f\n", float12_to_float(tanh_output));
  return 0;
}


