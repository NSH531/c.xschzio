#include "nifti1_io.h"
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

nifti_image *nifti_image_read_float12(const char *hname, int read_data) {
  nifti_image *nim = nifti_image_read(hname, read_data);
  if (!nim) {
    return NULL;
  }

  int nvox = nim->nx * nim->ny * nim->nz * nim->nt * nim->nu;
  float *data = (float *)nim->data;
  float12 *new_data = (float12 *)malloc(nvox * sizeof(float12));

  for (int i = 0; i < nvox; i++) {
    new_data[i] = float_to_float12(data[i]);
  }

  free(nim->data);
  nim->data = (void *)new_data;
  return nim;
}

int main(int argc, char *argv[])
{
// Check the number of arguments
if (argc != 2) {
printf("Usage: %s <filename>\n", argv[0]);
return 1;
}
  // Load the NIfTI file
nifti_image *nim = nifti_image_read(argv[1], 1);
if (!nim) {
    printf("Failed to load NIfTI file %s\n", argv[1]);
    return 1;
}

// Get the data type and number of elements
int datatype = nim->datatype;
int nvox = nim->nvox;

// Get a pointer to the data buffer
void *data = nim->data;

// Do something with the data

// Free the NIfTI image structure
nifti_image_free(nim);

return 0;
}

