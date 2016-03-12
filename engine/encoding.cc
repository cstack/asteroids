#include "encoding.h"

void print_hex_encoded(uint* data, size_t size) {
  uint num_parts = size / sizeof(uint);
  for (int i=0; i<num_parts; i++) {
    printf("%08x", data[i]);
  }
  printf("\n");
}

void read_hex_encoded(uint* destination, size_t size) {
  uint num_parts = size / sizeof(uint);
  for (int i=0; i<num_parts; i++) {
    scanf("%08x", destination + i);
  }
}