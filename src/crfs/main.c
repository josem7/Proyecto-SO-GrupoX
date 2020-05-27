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
  int result = cr_exists(1, "Baroque.mp3");
  printf("%d\n",result );
  //cr_directory(1);
  crFILE *cr_File = cr_open(2, "Hola.txt", 'w');
  delete_crFILE(cr_File);
}
