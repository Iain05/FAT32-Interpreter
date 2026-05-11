#include "fat32_utils.h"
#include <stdint.h>
#include <stdio.h>

BPB bpb;

int initBPB(FILE *disk) {
  fread(&bpb, sizeof(bpb), 1, disk);

  printf("jmpBoot: [0x%02X, 0x%02X, 0x%02X]\n", bpb.jmpBoot[0], bpb.jmpBoot[1],
         bpb.jmpBoot[2]);

  printf("OEMName: %.8s\n", (char *)bpb.OEMName);

  printf("BytesPerSec: %d", bpb.BytesPerSec);

  return 0;
}
