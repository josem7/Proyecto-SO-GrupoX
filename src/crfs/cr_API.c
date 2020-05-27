#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>
#include "cr_API.h"

// Global Variables
int PARTITION_SIZE = 536870912; //512 MB a bytes
int BLOCK_SIZE = 8192; // 8KB a bytes

// Struct de archivos


crFILE *create_crFILE(char fileName[29], unsigned int indexLocation, uint64_t size, unsigned int references)
{
    crFILE *cr_File = malloc(sizeof(crFILE));
    cr_File->name = fileName;
    cr_File->indexLocation = indexLocation;
    cr_File->size = size;
    cr_File->references = references;

    return cr_File;
}

void delete_crFILE(crFILE *cr_File) {
  free(cr_File);
}
// Funciones Utiles

// returns the first block that is free
unsigned next_free_block(unsigned disk){
  FILE *file;
  char *buffer;
  // cada disco tiene 65536 bloques 512MB / 8KB
  int cont = 65536 * (disk-1);
  int bitMapPointer = (disk-1) * PARTITION_SIZE + BLOCK_SIZE;
  file = fopen(MOUNTED_DISK, "rb");
  fseek(file, bitMapPointer, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);
  for (int index = 0; index < BLOCK_SIZE; index++) {
    unsigned int byte = buffer[index];
    for (size_t i = 0; i < 8; i++) {
      unsigned int bit = byte & 0x080;
      bit >>= 7;
      if (bit == 0) {
        free(buffer);
        fclose(file);
        return(cont);
      }
      cont += 1;
      byte <<= 1;
    }
  }
  free(buffer);
  fclose(file);
  printf("No Free Blocks\n");
  return(0);
}

// Funciones Pedidas
void cr_mount(char* diskname) {
  MOUNTED_DISK = diskname;
}
// imprime el block de directorio en bits
void cr_directory(unsigned disk) {
  FILE *file;
  char *buffer;
  file = fopen(MOUNTED_DISK, "rb");
  int directoryStartByte = (disk-1) * PARTITION_SIZE;
  fseek(file, directoryStartByte, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);

  // 8192 / 32 = 256 osea son 256 entradas
  for (int entry = 0; entry < 256; entry++) {
    for (int index = 0; index < 32; index++) {
      unsigned int byte = buffer[entry*32+index];
      for (size_t i = 0; i < 8; i++) {
        unsigned int bit = byte & 0x080;
        bit >>= 7;
        printf("%d",bit);
        byte <<= 1;
      }
        if (index % 32 == 31) {
            printf("\n");
        } else {
            printf(" ");
        }

      }
      printf("\n");
    }
  free(buffer);
  fclose(file);
}

// src = https://stackoverflow.com/questions/55288402/printing-bytes-read-from-binary-file-at-a-certain-file-position-c
// saqué este código de internet espero que funcione como creo
void cr_bitmap(unsigned disk, bool hex) {
  FILE *file;
  char *buffer;
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

int cr_exists(unsigned disk, char* filename) {
  FILE *file;
  char *buffer;
  char entryFileName[29];
  int result;
  file = fopen(MOUNTED_DISK, "rb");
  int directoryStartByte = (disk-1) * PARTITION_SIZE;
  fseek(file, directoryStartByte, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);

  // 8192 / 32 = 256 osea son 256 entradas
  for (int entry = 0; entry < 256; entry++) {
    for (int index = 3; index < 32; index++) {
      unsigned int byte = buffer[entry*32+index];
      entryFileName[index-3] = byte;
    }
    result = strcmp(filename, entryFileName);
    if (result == 0) {
      fclose(file);
      return(1);
    }
  }
  fclose(file);
  return(0);
}

void cr_ls(unsigned disk) {
  FILE *file;
  char *buffer;
  char entryFileName[29];
  int result;
  file = fopen(MOUNTED_DISK, "rb");
  printf("Files in DISK NUMBER: %d\n",disk);
  int directoryStartByte = (disk-1) * PARTITION_SIZE;
  fseek(file, directoryStartByte, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);

  // 8192 / 32 = 256 osea son 256 entradas
  for (int entry = 0; entry < 256; entry++) {
    for (int index = 3; index < 32; index++) {
      unsigned int byte = buffer[entry*32+index];
      entryFileName[index-3] = byte;
    }
    result = strcmp(entryFileName,"");
    if (result != 0) {
      printf("%s\n",entryFileName );
    }
  }
  free(buffer);
  fclose(file);

}

crFILE* cr_open(unsigned disk, char* filename, char mode){
  FILE *file;
  char *buffer;
  char entryFileName[29];
  int directoryEntry;
  unsigned int location;
  uint64_t size = 0;
  unsigned int references = 0;
  int result;

  if (mode == 'r') {
    if (cr_exists(disk, filename) == 1){
      if (filename[1] == '/'){
        int softlinkDisk = (int)filename[0] - 48;
        if (softlinkDisk >= 0 && softlinkDisk <= 4) {
          disk = softlinkDisk;
          filename += 2;
          printf("Soft Link to %s in disk : %d\n",filename, disk);
        }
      }
    }
    if (cr_exists(disk, filename) == 1){
      file = fopen(MOUNTED_DISK, "rb");
      int directoryStartByte = (disk-1) * PARTITION_SIZE;
      fseek(file, directoryStartByte, SEEK_SET);
      buffer = malloc(sizeof(char) * BLOCK_SIZE);
      fread(buffer, sizeof(char), BLOCK_SIZE, file);

      //Obtiene el nombre y la entrada en el directorio del archivo
      for (int entry = 0; entry < 256; entry++) {
        for (int index = 3; index < 32; index++) {
          unsigned int byte = buffer[entry*32+index];
          entryFileName[index-3] = byte;
        }
        result = strcmp(filename, entryFileName);
        if (result == 0) {
          directoryEntry = entry;
          break;
        }
      }
    // obtiene la ubicacion del index del archivo
    // el & 0x07F es para eliminar el bit de validez
    // No sé pq hay que hacer los otros & 0x0FF pero si no me da con puros 1 antes
    unsigned int byte1 = buffer[directoryEntry*32] & 0x07F;
    unsigned int byte2 = buffer[directoryEntry*32+1] & 0x0FF;
    unsigned int byte3 = buffer[directoryEntry*32+2] & 0x0FF;
    location = (byte1 << 16) + (byte2 << 8) + byte3;
    free(buffer);
    int indexBlockPointer = location * BLOCK_SIZE;
    fseek(file, indexBlockPointer, SEEK_SET);
    buffer = malloc(sizeof(char) * BLOCK_SIZE);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    //lee las referencias
    for (size_t i = 0; i < 4; i++) {
      unsigned int byte = buffer[i] & 0x0FF;
      references += byte;
      if (i != 3) {
        size <<= 8;
      }
    }
    // lee los 8 bytes de size
    for (size_t i = 4; i < 12; i++) {
      unsigned int byte = buffer[i] & 0x0FF;
      size += byte;
      if (i != 11) {
        size <<= 8;
      }
    }
    crFILE *cr_File = create_crFILE(entryFileName, location, size, references);
    printf("nombre : %s loc: %u, size %" PRIu64 " references : %u \n",cr_File->name, cr_File->indexLocation, cr_File->size, cr_File->references);
    free(buffer);
    fclose(file);
    return (cr_File);

    } else {
      //Sorry estaba muuy aburrido
      printf("Segmentation fault (core dumped)\n");
      sleep(1);
      printf("Broma....\n");
      printf("Archivo no existe\n");
      return(0);
    }
  } else if (mode == 'w'){
    if (cr_exists(disk, filename) == 0) {
      location = next_free_block(disk);
      crFILE *cr_File = create_crFILE(filename, location, size, references);
      printf("nombre : %s loc: %u, size %" PRIu64 " references : %u \n",cr_File->name, cr_File->indexLocation, cr_File->size, cr_File->references);
      return(cr_File);
    }else{
      printf("Archivo ya existe\n");
      return(0);
    }
  } else {
    printf("Invalid Mode\n");
    return(0);
  }

}

int cr_hardlink(unsigned disk, char* orig, char* dest) {
  FILE *file;
  char *buffer;
  char fileData[3];
  char entryFileData[32];
  char entryFileName[29];
  unsigned int location;
  file = fopen(MOUNTED_DISK, "rb+");
  if (file == NULL) {
    perror("Could not open file");
    exit(1);
  }

  int directoryPointer = (disk-1) * PARTITION_SIZE;
  fseek(file, directoryPointer, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);

  for (int entry = 0; entry < 256; entry++) {
    for (int index = 0; index < 32; index++) {
      unsigned int byte = buffer[entry * 32 + index];
      if (index <= 2) {
        fileData[index] = byte;
      } else if (index > 2) {
        entryFileName[index-3] = byte;
      }
    }
    if (strcmp(orig, entryFileName) == 0) {
      unsigned int byte1 = buffer[entry*32] & 0x07F;
      unsigned int byte2 = buffer[entry*32+1] & 0x0FF;
      unsigned int byte3 = buffer[entry*32+2] & 0x0FF;
      location = (byte1 << 16) + (byte2 << 8) + byte3;
      int indexBlockPointer = location * BLOCK_SIZE;
      fseek(file, indexBlockPointer, SEEK_SET);
      char *strNum;
      strNum = malloc(sizeof(char) * 4);
      fread(strNum, sizeof(char), 4, file);
      int num = strtol(strNum, & strNum, 10) + 1;
      fseek(file, indexBlockPointer, SEEK_SET);
      fwrite(&num, sizeof(int), 1, file);

      size_t j = 0;
      for (int i = 0; i < 3; i++) {
        entryFileData[j++] = fileData[i];
      }
      entryFileData[j] = 0;
      break;
    }
  }

  for (int entry = 0; entry < 256; entry++) {
    unsigned int firstByte = buffer[entry*32];
    unsigned int validity = (firstByte & 10000000) >> 7;
    if (validity == 0) {
      strcat(entryFileData, dest);
      int entryPointer = (disk-1) * PARTITION_SIZE + entry*32;
      fseek(file, entryPointer, SEEK_SET);
      fwrite(&entryFileData, sizeof(entryFileData), 1, file);
      fclose(file);
      break;
    }
  }
  free(buffer);
}
