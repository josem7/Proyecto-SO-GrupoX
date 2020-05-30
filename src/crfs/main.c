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
  cr_read(file);
}
