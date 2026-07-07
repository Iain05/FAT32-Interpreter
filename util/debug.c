#include <limits.h>
#include <stdio.h>

#include "../fat32_utils.h"

void print_bits(unsigned long n, int num_bits) {
  for (int i = num_bits - 1; i >= 0; i--) {
    int bit = (n >> i) & 1;
    printf("%d", bit);
    if (i % 8 == 0 && i != 0)
      printf(" ");
  }
  printf("\n");
}

void print_bits_str(char *s) {
  while (*s != '\0') {
    printf("%c: ", *s);
    print_bits(*s, sizeof(*s) * CHAR_BIT);
    s++;
  }
}

void print_fat_entry_data(fat_t entry) { (void)entry; }
