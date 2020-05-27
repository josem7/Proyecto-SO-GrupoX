#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include "cr_API.h"

int main (int argc, char* argv[]) {
  char* diskName =  argv[1];
  cr_mount(diskName);
  printf("%s\n",MOUNTED_DISK);
  //cr_bitmap(1, true);
  //cr_bitmap(2, false);
  int result = cr_exists(1, "Baroque.mp3");
  printf("%d\n",result );
  cr_ls(3);
  //cr_directory(1);
  crFILE *cr_File = cr_open(4, "1/aaaa.gif", 'r');
  if (cr_File != 0) {
    delete_crFILE(cr_File);
  }
}
