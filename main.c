#include "fat32_utils.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  (void)argc;

  FILE *disk = fopen(argv[1], "r");

  if (disk == NULL) {
    perror("Error");
    return 1;
  }

  initBPB(disk);

  fclose(disk);
  return 0;
}
