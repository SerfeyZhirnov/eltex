#include <stdio.h>

int main() {
  int a = 0xDDCCBBAA;
  unsigned char c;

  c = (a & 0xFF);
  printf("First byte of a = %02X\n", c);
  
  c = ((a >> 8) & 0xFF);
  printf("Second byte of a = %02X\n", c);
  
  c = ((a >> 16) & 0xFF);
  printf("Third byte of a = %02X\n", c);

  c = 0xAB;
  a = ((a & 0x00FFFFFF) | (c << 24));
  printf("Bit-changed number %X\n", a);

  return 0;
}