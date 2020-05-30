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
  cr_ls(1);
  crFILE *file = cr_open(1, "guides.txt", 'r');
  char *buffer = malloc(sizeof(char) * 16000);
  for (int i = 0; i < 16000; i++)
  {
    buffer[i] = NULL;
  }

  int result = cr_read(file, buffer, 16000);
  free(buffer);
  printf("Result: %d\n", result);
  cr_close(file);
}
