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
#include <math.h>

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

// Raise x to the power of n
float12 float12_pow(float12 x, int n) {
  float12 result = float_to_float12(1);
  for (int i = 0; i < n; i++) {
    result = float12_mul(result, x);
  }
  return result;
}

// Compute the factorial of n
float12 float12_factorial(int n) {
  float12 result = float_to_float12(1);
  for (int i = 2; i <= n; i++) {
    result = float12_mul(result, float_to_float12(i));
  }
  return result;
}

// Approximate the hyperbolic tangent of x using a Taylor series
float12 float12_tanh(float12 x) {
  float12 result = x;
  float12 x_pow = x;
  float12 n_factorial = float_to_float12(1);
  for (int n = 1; fabs(float12_to_float(x_pow)) > 1e-6; n++) {
    x_pow = float12_mul(x_pow, x);
    x_pow = float12_mul(x_pow, x);
    n_factorial = float12_mul(n_factorial, float_to_float12(2 * n));
    n_factorial = float12_mul(n_factorial, float_to_float12(2 * n + 1));
    result = result + float12_div(x_pow, n_factorial);
  }
  return result;
}

int main(void) {
  float12 x = float_to_float12(1);
  printf("tanh(%f) = %f\n", float12_to_float(x), float12_to_float(float12_tanh(x)));
  return 0;
}

int main(void) {
  float12 a = float_to_float12(2.75);
  float12 b = float_to_float12(1.25);
  printf("a: %f\n", float12_to_float(a));
  printf("b: %f\n", float12_to_float(b));
  printf("a + b: %f\n", float12_to_float(float12_add(a, b)));
  printf("a - b: %f\n", float12_to_float(float12_sub(a, b)));
  printf("a * b: %f\n", float12_to_float(float12_mul(a, b)));

}
