#include <stdio.h>

typedef struct {
    unsigned short fraction: 6;
    unsigned short exponent: 5;
    unsigned short sign: 1;
} float12;

float12 add(float12 a, float12 b) {
    // Implementation of addition operation
    // ...
}

int main() {
    float12 a, b, c;
    // Initialize a and b
    // ...
    c = add(a, b);
    return 0;
}
