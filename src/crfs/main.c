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
  //cr_ls(3);
  //crFILE *file = cr_open(1, "guides.txt", 'r');
  //cr_close(file);
  //void *buffer = malloc(16000);
  //cr_read(file, buffer, 16000);
  //free(buffer);
  //cr_rm(3, "Program.mkv");
  //cr_bitmap(3,0);
  //crFILE *file = cr_open(1, "guides.txt", 'r');
  //void *buffer = malloc(16000);
  //int result = cr_read(file, buffer, 16000);
  //free(buffer);
  cr_unload(2, "yes.mp3", "yes.mp3");
  //cr_unload(1, "text.txt", "text.txt");

  // cr_load(4, "src/crfs");
  // cr_ls(4);
  cr_unload(3, "README.md", "prueba.md");
}
