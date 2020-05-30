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
  //crFILE *file = cr_open(1, "guides.txt", 'r');
  //void *buffer = malloc(16000);
  //int result = cr_read(file, buffer, 16000);
  //free(buffer);
  cr_unload(1, "guides.txt", "guides.txt");
  //cr_unload(1, "text.txt", "text.txt");
}
