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

int main(void) {
  float12 a = float_to_float12(2.75);
  float12 b = float_to_float12(1.25);
  printf("a: %f\n", float12_to_float(a));
  printf("b: %f\n", float12_to_float(b));
  printf("a + b: %f\n", float12_to_float(float12_add(a, b)));
  printf("a - b: %f\n", float12_to_float(float12_sub(a, b)));
  printf("a * b: %f\n", float12_to_float(float12_mul(a, b)));
  printf("a / b: %f\n", float12_to_float(float12_
