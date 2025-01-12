#include <endian.h>
#include <stdio.h>
#include <stdint.h>

void print_system_byteorder_using_memory () {
  uint32_t value = 0x12345678;
  uint8_t *ptr = (uint8_t *)&value;
  char *byteorder;

  if (ptr[0] == 0x78)
    byteorder = "Little Endian";
  else if (ptr[0] == 0x12)
    byteorder = "Big Endian";
  else
    byteorder = "Unknown";
  printf("System byte order is %s.\n", byteorder);

  for (int i = 0; i < 4; i++) {
    printf("in %p (ptr[%d]): %x\n", &ptr[i], i, ptr[i]);
  }
}

void print_system_byteorder_using_macro () {
  char *byteorder;

  #if __BYTE_ORDER == __LITTLE_ENDIAN
    byteorder = "Little Endian";
  #elif __BYTE_ORDER == __BIG_ENDIAN
    byteorder = "Big Endian";
  #else
   byteorder = "Unknown";
  #endif
  printf("System byte order is %s.\n", byteorder);
}

int main () {
  // printf("Inspected by memory order, ");
  // print_system_byteorder_using_memory ();
  printf("Inspected by endian macro, ");
  print_system_byteorder_using_macro ();
}
