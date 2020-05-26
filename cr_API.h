#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
// Global Variables
int partitionSize = 536870912; //512 MB a bytes
int blockSize = 8192; // 8KB a bytes
char* mountedDisk;

// Asigna la variable global mountedDisk
void cr_mount(char* diskname){
  mountedDisk = diskname;
}

// src = https://stackoverflow.com/questions/55288402/printing-bytes-read-from-binary-file-at-a-certain-file-position-c
// saqué este código de internet espero que funcione como creo
void cr_bitmap(unsigned disk, bool hex){
  int bitMapPointer = (1-disk) * partitionSize + blockSize;
  FILE *file;
  char *buffer;
  int cont = 0;
  file = fopen(mountedDisk, "rb");
  if (file == NULL) {
      perror("Could not open file");
      exit(1);
  }
  fseek(file, bitMapPointer, SEEK_SET);
  buffer = malloc(sizeof(char) * blockSize);
  fread(buffer, sizeof(char), blockSize, file);
  if (hex) {
    for (int index = 0; index < blockSize; index++) {
        printf("%02X", ((unsigned int) buffer[index]) & 0x0FF);
        cont += 1;
        if (index % 8 == 7) {
            printf("\n");
        }else {
            printf(" ");
        }
    }
  }else {
      for (int index = 0; index < blockSize; index++) {

        unsigned int byte = buffer[index];
        for (size_t i = 0; i < 8; i++) {
          unsigned int bit = byte & 0x01;
          printf("%d",bit);
          byte >>= 1;
        }
          if (index % 8 == 7) {
              printf("\n");
          }else {
              printf(" ");
          }
    }

  }

  free(buffer);
  printf("%d\n",cont );
}
