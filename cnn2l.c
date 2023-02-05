#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    unsigned short fraction: 6;
    unsigned short exponent: 5;
    unsigned short sign: 1;
} float12;

float12 add(float12 a, float12 b) {
    float12 result;
    unsigned short common_exponent = (a.exponent > b.exponent) ? a.exponent : b.exponent;
    unsigned short fraction_a = a.fraction >> (a.exponent - common_exponent);
    unsigned short fraction_b = b.fraction >> (b.exponent - common_exponent);
    unsigned short sum = fraction_a + fraction_b;
    if (sum & 0x400) {
        result.fraction = (sum >> 1) & 0x3FF;
        result.exponent = common_exponent + 1;
    } else {
        result.fraction = sum & 0x3FF;
        result.exponent = common_exponent;
    }
    result.sign = (a.sign == b.sign) ? a.sign : (fraction_a + fraction_b >= 0x400);
    return result;
}
float12 sub(float12 a, float12 b) {
    b.sign = !b.sign;
    return add(a, b);
}
float12 mul(float12 a, float12 b) {
    float12 result;
    unsigned long long product = (unsigned long long) a.fraction * b.fraction;
    int shift = 0;
    while (product & 0xFFC0000000000000ull) {
        product >>= 1;
        shift++;
    }
    result.fraction = product & 0x3FF;
    result.exponent = a.exponent + b.exponent - shift;
    result.sign = (a.sign == b.sign) ? 0 : 1;
    return result;
}
float12 div(float12 a, float12 b) {
    float12 result;
    unsigned long long dividend = (unsigned long long) a.fraction << 10;
    unsigned short divisor = b.fraction;
    int shift = 0;
    while (divisor & 0x400) {
        divisor >>= 1;
        shift++;
    }
    result.fraction = dividend / divisor;
    result.exponent = a.exponent - b.exponent - shift;
    result.sign = (a.sign == b.sign) ? 0 : 1;
    return result;
}
#include <math.h>

#define EXP_A (1048576 / log(2))
#define EXP_C 60801

float12 exp_f12(float12 x) {
    float12 result;
    int k = (int) ((x.fraction + x.exponent * EXP_A) * EXP_C);
    result.fraction = ldexp(1, k & 1023);
    result.exponent = k >> 10;
    result.sign = 0;
    return result;
}


int main() {
    float12 a, b, c;
    // Initialize a and b
    // ...
    c = add(a, b);
    return 0;
}
#define layer1_nodes 5
#define layer2_nodes 5

#include <math.h>

float12 double_to_float12(double value) {
    float12 result;
    result.fraction = value * 1024.0;
    result.exponent = 0;
    result.sign = (value < 0) ? 1 : 0;
    return result;
}
float12 float12_from_double(double v){
return double_to_float12(v);
}
float12 sigmoid_f12(float12 x) {
    double x_double = (double) x.fraction / 1024.0 + x.exponent;
    double sigmoid_double = 1.0 / (1.0 + exp(-x_double));
    return double_to_float12(sigmoid_double);
}
#include <nifti1_io.h>
#include <stdio.h>

int main1(int argc, char *argv[])
{
    nifti_image *nim;

    if (argc < 2) {
        printf("Usage: %s <input NIfTI file>\n", argv[0]);
        return 1;
    }

    nim = nifti_image_read(argv[1], 1);
    if (!nim) {
        printf("Failed to read NIfTI file\n");
        return 1;
    }

    // Do something with the NIfTI data

    nifti_image_free(nim);
    return 0;
}

int main() {
// Initialize weights and biases for layer 1
float12  w1[layer1_nodes][layer2_nodes]= {{float12_from_double(0.1), float12_from_double(0.2), float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5)},
{float12_from_double(0.2), float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5), float12_from_double(0.6)},
{float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5), float12_from_double(0.6), float12_from_double(0.7)},
{float12_from_double(0.4), float12_from_double(0.5), float12_from_double(0.6), float12_from_double(0.7), float12_from_double(0.8)},
{float12_from_double(0.5), float12_from_double(0.6), float12_from_double(0.7), float12_from_double(0.8), float12_from_double(0.9)}};
}};

double b1[layer1_nodes] = {float12_from_double(0.1), float12_from_double(0.2), float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5)}

// Initialize weights and biases for layer 2
double w2[layer2_nodes] = {float12_from_double(0.1), float12_from_double(0.2), float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5)};
double b2 = 0.1;

// Input layer with 5 nodes
double input[layer1_nodes] = {float12_from_double(0.1), float12_from_double(0.2), float12_from_double(0.3), float12_from_double(0.4), float12_from_double(0.5)};

// Output layer with 1 node
double output = 0.0;

// Layer 1 computations
double layer1_output[layer1_nodes];
for (int i = 0; i < layer1_nodes; i++) {
    layer1_output[i] = 0.0;
    for (int j = 0; j < layer2_nodes; j++) {
        layer1_output[i] += w1[i][j] * input[j];
    }
    layer1_output[i] += b1[i];
    layer1_output[i] = sigmoid(layer1_output[i]);
}

// Layer 2 computations
for (int i = 0; i < layer2_nodes; i++) {
    output += w2[i] * layer1_output[i];
}
output += b2;
output = sigmoid(output);

// Final output
printf("Output: %lf\n", output);

return 0;
