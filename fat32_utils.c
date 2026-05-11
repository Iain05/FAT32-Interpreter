#include "fat32_utils.h"
#include "fat32_init.h"

int initBPB(FILE *disk) {
  initialize(disk);
  return 0;
}
