#ifndef FAT32_UTILS_H
#define FAT32_UTILS_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t jmpBoot[3];
  uint8_t OEMName[8];
  uint16_t BytesPerSec;
  uint8_t SecPerClus;
  uint16_t RsvdSecCnt;
  uint8_t NumFATs;
  uint16_t RootEntCnt;
  uint16_t TotSec16;
  uint8_t Media;
} __attribute__((packed)) BPB;

int initBPB(FILE *disk);

#endif // !FAT32_UTILS_H
