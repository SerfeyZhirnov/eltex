#include <stdio.h>

int main() {
  int a = 0xAABBCCDD;
  printf("Original number - %X\n", a);
  char *ptr = (char *)&a;

  for (int i = 0; i < 4; ++i, ++ptr) {
    printf("%d - byte = %2X\n", i + 1, *ptr & 0xFF);
  }

  --ptr, *ptr = 0xFF;
  printf("Change first byte by pointer - %X\n", a);

  return 0;
}