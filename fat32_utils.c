#include <string.h>
#include "fat32_utils.h"
#include "fat32_init.h"
#include "debug.h"

#define EOC_MAX 0x0FFFFFFF
#define EOC_MIN 0x0FFFFFF8
#define EOC(a) (((a) > EOC_MIN && (a) < EOC_MAX) ? 1 : 0)

FILE *disk;
BPB bpb;
uint16_t fat_start_sector;
uint16_t data_start_sector;
uint16_t bytes_per_cluster;
uint32_t current;

uint32_t cluster_to_offset(uint32_t N) {
  return (data_start_sector + (N - 2) * bpb.SecPerClus) * bpb.BytesPerSec;
}

uint32_t next_cluster(uint32_t N) {
  uint32_t next = (fat_start_sector * bpb.BytesPerSec) + (N * 4);
  return next & 0x0FFFFFFF;
}

int initBPB(FILE *d) {
  disk = d;
  int res = initialize(disk, &bpb);
  if (res != 0) {
    printf("Error during initialization\n");
    return 1;
  } 

  fat_start_sector = bpb.RsvdSecCnt;
  data_start_sector = bpb.RsvdSecCnt + (bpb.NumFATs * bpb.FATSz32);
  bytes_per_cluster = bpb.BytesPerSec * bpb.SecPerClus;

  current = bpb.RootClus;
  fseek(disk, cluster_to_offset(bpb.RootClus), SEEK_SET);

  return 0;
}

int parseFATEntry(FAT entry, char *name) {
    uint8_t first = (uint8_t)entry.Name[0];

    if (first == 0x00) return 1;   // end of directory
    if (first == 0xE5) return 2;   // deleted, skip
    if (entry.Attr == 0x0F) return 3;   // LFN
    
    memcpy(name, (char *)entry.Name, 11);

    return 0;
}

int list() {
    FAT entry;
    int entries_per_cluster = bytes_per_cluster >> 5;

    while (!EOC(current)) {
        fseek(disk, cluster_to_offset(current), SEEK_SET);

        for (int i = 0; i < entries_per_cluster; i++) {
            fread(&entry, sizeof(FAT), 1, disk);
            char name[12];
            name[11] = '\0';
            int retval = parseFATEntry(entry, name);
            if (retval == 1) return 0;
            if (retval == 0) {
                printf("%.11s\n", name);
                print_bits_str(name);
            }
        }
        current = next_cluster(current);
    }
    return 0;
}

// i think we need to print the raw bytes to see whats actually going on in the entry.Name
int cd_single(char *dir) {
    (void) dir;
    return 0;
}
