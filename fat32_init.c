#include "fat32_init.h"
#include <stdio.h>

BPB initialize(FILE *disk) {
  BPB bpb;
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

  return bpb;
}
