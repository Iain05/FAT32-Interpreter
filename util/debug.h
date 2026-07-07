#ifndef DEBUG_H
#define DEBUG_H

#include "../fat32_utils.h"
#include <limits.h>

void print_bits(unsigned long n, int num_bits);

void print_bits_str(char *s);

void print_fat_entry_data(fat_t entry);

#endif // ! DEBUG_H
