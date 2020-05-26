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