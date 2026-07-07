#include "fat32_utils.h"
#include "debug.h"
#include "fat32_init.h"
#include "utf16.h"
#include <assert.h>
#include <string.h>
#include <uchar.h>

#define LFN_BUFFER_SIZE 255
#define EOC_MAX 0x0FFFFFFF
#define EOC_MIN 0x0FFFFFF8
#define EOC(a) (((a) > EOC_MIN && (a) < EOC_MAX) ? 1 : 0)

FILE *disk;
bpb_t bpb;
uint16_t fat_start_sector;
uint16_t data_start_sector;
uint16_t bytes_per_cluster;
uint32_t current;

char16_t lfn_buffer[LFN_BUFFER_SIZE];
uint8_t lfn_buffer_head = LFN_BUFFER_SIZE;

// at some point I could probably implement like a linkedlist of directories
// every time the user cds so we dont have to search every time. But that
// sounds like later me's problem to implement. And frankly it doesn't matter

uint32_t cluster_to_offset(uint32_t N) {
  return (data_start_sector + (N - 2) * bpb.SecPerClus) * bpb.BytesPerSec;
}

uint32_t next_cluster(uint32_t N) {
  uint32_t fat_offset = (fat_start_sector * bpb.BytesPerSec) + (N * 4);
  uint32_t next;
  fseek(disk, fat_offset, SEEK_SET);
  fread(&next, sizeof(next), 1, disk);
  return next & 0x0FFFFFFF;
}

uint32_t cluster_of_directory(fat_t entry) {
  if ((entry.Attr & 0x10) == 0)
    return 0;
  uint32_t cluster = (entry.FstClusHI << 16) | entry.FstClusLO;
  if (cluster == 0)
    return bpb.RootClus;
  return cluster;
}

int init_bpb(FILE *d) {
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

int parse_lfn_entry(lfn_t entry) {
  // yeah this is kinda dumb ill make it nicer later its probably temporary
  if (entry.Ord & 0x40)
    assert(lfn_buffer_head == LFN_BUFFER_SIZE);
  lfn_buffer_head -= 13;
  memcpy(&lfn_buffer[lfn_buffer_head], entry.Name, sizeof(entry.Name));
  memcpy(&lfn_buffer[lfn_buffer_head + 5], entry.Name2, sizeof(entry.Name2));
  memcpy(&lfn_buffer[lfn_buffer_head + 11], entry.Name3, sizeof(entry.Name3));
  if ((entry.Ord & 0x1F) == 0x01)
    return 0;
  return 1;
}

int parse_fat_entry(fat_t entry) {
  uint8_t first = (uint8_t)entry.Name[0];
  int retval;

  if (first == 0x00)
    return 1; // end of directory
  if (first == 0xE5)
    return 2; // deleted, skip

  if ((entry.Attr & 0x3F) == 0x0F) {
    retval = parse_lfn_entry(*((lfn_t *)&entry));
    if (retval == 0)
      return 3; // Last LFN
    return 4;   // LFN
  }

  /* int end = 11; */
  /* while (end > 0 && entry.Name[end - 1] == ' ') { */
  /*   end--; */
  /* } */
  /**/
  /* memcpy(name, entry.Name, end); */
  /* name[end] = '\0'; */

  return 0;
}

// return the number of fat entries read or 0 for the last entry
int parse_file() {
  fat_t entry;
  int retval;
  int num_entries = 0;

  do {
    fread(&entry, sizeof(fat_t), 1, disk);
    retval = parse_fat_entry(entry);

    if (retval == 1) // end of directory
      return 0;

    if (retval == 3) {
      print_utf16(lfn_buffer + lfn_buffer_head,
                  LFN_BUFFER_SIZE - lfn_buffer_head);
      lfn_buffer_head = LFN_BUFFER_SIZE;
      printf("\n");
    }
    num_entries++;
  } while (retval == 4);

  return num_entries;
}

int list() {
  printf("\nList (cluster %d):\n", current);

  int retval;
  int entries_per_cluster = bytes_per_cluster >> 5;

  while (!EOC(current)) {
    fseek(disk, cluster_to_offset(current), SEEK_SET);

    int parsed_entries = 0;
    do {
      retval = parse_file();
      if (retval == 0) {
        return 0;
      }
      parsed_entries += retval;
    } while (parsed_entries < entries_per_cluster);

    if (parsed_entries > entries_per_cluster)
      printf("Something has gone very VERY wrong\n");

    current = next_cluster(current);
  }
  return 0;
}

/* int cd_single(char *dir) { */
/*   printf("\nCD %s:\n", dir); */
/*   fat_t entry; */
/*   int entries_per_cluster = bytes_per_cluster >> 5; */
/**/
/*   while (!EOC(current)) { */
/*     fseek(disk, cluster_to_offset(current), SEEK_SET); */
/**/
/*     for (int i = 0; i < entries_per_cluster; i++) { */
/*       fread(&entry, sizeof(fat_t), 1, disk); */
/*       char name[12]; */
/*       int retval = parse_fat_entry(entry, name); */
/**/
/*       if (retval == 1) { */
/*         printf("Directory %s does not exist\n", dir); */
/*         return 0; */
/*       } */
/**/
/*       if (retval != 0) { */
/*         continue; */
/*       } */
/**/
/*       if (strcmp(dir, name) == 0) { */
/*         current = cluster_of_directory(entry); */
/*         return 0; */
/*       } */
/*     } */
/*     current = next_cluster(current); */
/*   } */
/*   printf("Directory %s does not exist\n", dir); */
/*   return 0; */
/* } */
