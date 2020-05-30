#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#pragma once

// Global Variables
int PARTITION_SIZE;
int BLOCK_SIZE;
char *MOUNTED_DISK;

//Estructuras
typedef struct crFILE
{
  char *name;
  unsigned int indexLocation;
  unsigned int disk;
  uint64_t size;
  unsigned int references;
} crFILE;
//Funciones Utiles
void delete_crFILE(crFILE *cr_File);

// Asigna la variable global MOUNTED_DISK
void cr_mount(char *diskname);

// Función para imprimir el bitmap
void cr_bitmap(unsigned disk, bool hex);

// Crea un hardlink entre orig y dest
int cr_hardlink(unsigned disk, char *orig, char *dest);

// Crea un softlink entre orig y dest
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char *orig);

// Función para ver si un archivo existe en una determinada partición. Retorna 1
//si el archivo existe y 0 en caso contrario.
int cr_exists(unsigned disk, char *filename);

//Función para listar los elementos de una partición. Imprime en pantalla
//los nombres de todos los archivos contenidos en la partición indicado por disk
void cr_ls(unsigned disk);

void cr_directory(unsigned disk);

int cr_close(crFILE *file_desc);

//Función para abrir un archivo
//y se retorna un nuevo crFILE* que lo representa.
crFILE *cr_open(unsigned disk, char *filename, char mode);

int cr_write(crFILE *file_desc, void *buffer, int nbytes);

int cr_read(crFILE *file_desc, void *buffer, int n_bytes);

//Función que se encarga de co-piar un archivo, la partición completa (
//disk ∈ {1, ..., 4}) o el disco completo (disk = 0)
int cr_unload(unsigned disk, char* orig, char* dest);
