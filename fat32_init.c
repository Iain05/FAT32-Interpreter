#include "fat32_init.h"
#include <stdio.h>

int initialize(FILE *disk, bpb_t *retval) {
  bpb_t bpb;
  fread(&bpb, sizeof(bpb), 1, disk);

  printf("jmpBoot: [0x%02X, 0x%02X, 0x%02X]\n", bpb.jmpBoot[0], bpb.jmpBoot[1],
         bpb.jmpBoot[2]);

  // We store the name as an array of ints and print a max 8 characters
  // the standard makes no requirement that OEMName is null terminating
  printf("OEMName: %.8s\n", (char *)bpb.OEMName);

  printf("BytesPerSec: %d\n", bpb.BytesPerSec);

  printf("SecPerClus: %d\n", bpb.SecPerClus);

  printf("RsvdSecCnt: %d\n", bpb.RsvdSecCnt);

  printf("NumFATs: %d\n", bpb.NumFATs);

  printf("RootEntCnt: %d\n", bpb.RootEntCnt);

  printf("TotSec16: %d\n", bpb.TotSec16);

  printf("Media: 0x%02X\n", bpb.Media);

  printf("FATSz16: %d\n", bpb.FATSz16);

  printf("SecPerTrk: %d\n", bpb.SecPerTrk);

  printf("NumHeads: %d\n", bpb.NumHeads);

  printf("HiddSec: %d\n", bpb.HiddSec);

  printf("TotSec32: %d\n", bpb.TotSec32);

  fseek(disk, 36, SEEK_SET);
  printf("\n-----Data fields for FAT32 (offset 36)-----\n");

  printf("FATSz32: %d\n", bpb.FATSz32);

  printf("ExtFlags: %b\n", bpb.FATSz32);

  printf("RootClus: %d\n", bpb.RootClus);

  *retval = bpb;
  return 0;
}
