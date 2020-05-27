#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include "cr_API.h"

// Global Variables
int PARTITION_SIZE = 536870912; //512 MB a bytes
int BLOCK_SIZE = 8192; // 8KB a bytes

// Struct de archivos
typedef struct crFILE {
  char *name;
  char *location;
  int size;
  unsigned references;
} crFILE;

void cr_mount(char* diskname) {
  MOUNTED_DISK = diskname;
}

// src = https://stackoverflow.com/questions/55288402/printing-bytes-read-from-binary-file-at-a-certain-file-position-c
// saqué este código de internet espero que funcione como creo
void cr_bitmap(unsigned disk, bool hex) {
  FILE *file;
  char *buffer;
  int cont = 0;
  file = fopen(MOUNTED_DISK, "rb");
  if (file == NULL) {
      perror("Could not open file");
      exit(1);
  }
  if (disk > 0 && disk <= 4) {
    printf("DISK NUMBER: %d\n",disk);
    int bitMapPointer = (disk-1) * PARTITION_SIZE + BLOCK_SIZE;
    fseek(file, bitMapPointer, SEEK_SET);
    buffer = malloc(sizeof(char) * BLOCK_SIZE);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    if (hex) {
      for (int index = 0; index < BLOCK_SIZE; index++) {
          printf("%02X", ((unsigned int) buffer[index]) & 0x0FF);
          cont += 1;
          if (index % 16 == 15) {
              printf("\n");
          }else {
              printf(" ");
          }
      }
    } else {
        for (int index = 0; index < BLOCK_SIZE; index++) {

          unsigned int byte = buffer[index];
          for (size_t i = 0; i < 8; i++) {
            unsigned int bit = byte & 0x080;
            bit >>= 7;
            printf("%d",bit);
            byte <<= 1;
          }
            if (index % 16 == 15) {
                printf("\n");
            } else {
                printf(" ");
            }
      }

    }
    free(buffer);
  } else if(disk == 0) {
    for (unsigned z = 1; z < 5; z++) {
      printf("DISK NUMBER: %d\n",z);
      int bitMapPointer = (z-1) * PARTITION_SIZE + BLOCK_SIZE;
      fseek(file, bitMapPointer, SEEK_SET);
      buffer = malloc(sizeof(char) * BLOCK_SIZE);
      fread(buffer, sizeof(char), BLOCK_SIZE, file);
      if (hex) {
        for (int index = 0; index < BLOCK_SIZE; index++) {
            printf("%02X", ((unsigned int) buffer[index]) & 0x0FF);
            cont += 1;
            if (index % 16 == 15) {
                printf("\n");
            } else {
                printf(" ");
            }
        }
      } else {
          for (int index = 0; index < BLOCK_SIZE; index++) {

            unsigned int byte = buffer[index];
            for (size_t i = 0; i < 8; i++) {
              unsigned int bit = byte & 0x080;
              bit >>= 7;
              printf("%d",bit);
              byte <<= 1;
            }
              if (index % 16 == 15) {
                  printf("\n");
              } else {
                  printf(" ");
              }

            }
      }
      free(buffer);
    }
  }

  fclose(file);
}

int cr_hardlink(unsigned disk, char* orig, char* dest) {
  FILE *file;
  char *buffer;
  file = fopen(MOUNTED_DISK, "rb");
  if (file == NULL) {
    perror("Could not open file");
    exit(1);
  }

  int bitMapPointer = (disk-1) * PARTITION_SIZE + BLOCK_SIZE;
  fseek(file, bitMapPointer, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);
}

int cr_softlink(unsigned disk_orig, unsigned disk_dest, char * orig, char * dest) {
  //
}
