#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include "cr_API.h"

int main(int argc, char *argv[])
{
  char *diskName = argv[1];
  cr_mount(diskName);
  printf("%s\n", MOUNTED_DISK);
  //cr_ls(1);
  crFILE *file = cr_open(1, "escribir10.txt", 'w');
  char *buffer = malloc(sizeof(char) * 20000000 + 1);
  for (int i = 0; i < 20000000; i++)
  {
    buffer[i] = 65;
  }
  buffer[20000000] = 0;
  //int result = cr_read(file, buffer, 16000);
  //printf("Result: %d\n", result);
  cr_write(file, buffer, 20000000);
  free(buffer);
  cr_close(file);
  //cr_bitmap(1, 0);
}
