#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#pragma once


// Global Variables
int PARTITION_SIZE;
int BLOCK_SIZE;
char* MOUNTED_DISK;

// Asigna la variable global MOUNTED_DISK
void cr_mount(char* diskname);

// Función para imprimir el bitmap
void cr_bitmap(unsigned disk, bool hex);

// Función para ver si un archivo existe en una determinada partición. Retorna 1
//si el archivo existe y 0 en caso contrario.
int cr_exists(unsigned disk, char* filename);
