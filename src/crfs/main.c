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
  int result = cr_exists(1, "Baroque.mp3");
  printf("result: %d\n", result);
  crFILE *file = cr_open(1, "QPC.gif", 'r');
  cr_close(file);
  // cr_bitmap(1, 0);
  cr_hardlink(1, "text.txt", "texto4.txt");
  cr_softlink(1,4, "Baroque.mp3");
  //cr_ls(1);
  // cr_bitmap(1, 0);
}
