#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define layer1_nodes 5
#define layer2_nodes 5

double sigmoid(double x) {
return 1.0 / (1.0 + exp(-x));
}

int main() {
// Initialize weights and biases for layer 1
double w1[layer1_nodes][layer2_nodes] = {{0.1, 0.2, 0.3, 0.4, 0.5},
{0.2, 0.3, 0.4, 0.5, 0.6},
{0.3, 0.4, 0.5, 0.6, 0.7},
{0.4, 0.5, 0.6, 0.7, 0.8},
{0.5, 0.6, 0.7, 0.8, 0.9}};

scss
Copy code
double b1[layer1_nodes] = {0.1, 0.2, 0.3, 0.4, 0.5};

// Initialize weights and biases for layer 2
double w2[layer2_nodes] = {0.1, 0.2, 0.3, 0.4, 0.5};
double b2 = 0.1;

// Input layer with 5 nodes
double input[layer1_nodes] = {0.1, 0.2, 0.3, 0.4, 0.5};

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
