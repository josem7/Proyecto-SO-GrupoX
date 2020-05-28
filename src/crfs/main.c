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
  int result = cr_exists(1, "Baroque.mp3");
  printf("%d\n", result);
  crFILE *file = cr_open(1, "QPC.gif", 'r');
  cr_close(file);
  cr_hardlink(1, "text.txt", "texto.txt");
  cr_ls(1);
}
