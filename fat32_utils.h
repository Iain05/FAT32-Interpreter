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
  uint16_t FATSz16;
  uint16_t SecPerTrk;
  uint16_t NumHeads;
  uint32_t HiddSec;
  uint32_t TotSec32;
  // FAT32 Specific Data fields
  uint32_t FATSz32;
  uint16_t ExtFlags;
  uint8_t FSVer[2];
  uint32_t RootClus;
  uint16_t FSInfo;
  uint16_t BkBootSec;
  uint8_t Reserved[12];
  // rest is boot sector which i dont wanna touch
} __attribute__((packed)) bpb_t;

typedef struct {
  uint8_t Name[11];
  uint8_t Attr;
  uint8_t NTRes;
  uint8_t CrtTimeTenth;
  uint16_t CrtTime;
  uint16_t CrtDate;
  uint16_t LstAccDate;
  uint16_t FstClusHI;
  uint16_t WrtTime;
  uint16_t WrtDate;
  uint16_t FstClusLO;
  uint32_t FileSize;
} __attribute__((packed)) fat_t;

typedef struct {
  uint8_t Ord;
  uint16_t Name[5];
  uint8_t Attr;
  uint8_t Type;
  uint8_t Chksum;
  uint16_t Name2[6];
  uint16_t Name3[2];
} __attribute__((packed)) lfn_t;

int init_bpb(FILE *disk);

int list();

int cd_single(char *dir);

#endif // !FAT32_UTILS_H
