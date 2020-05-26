#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "cr_API.h"

int main (int argc, char* argv[]) {
  char* diskName =  argv[1];
  cr_mount(diskName);
  printf("%s\n",mountedDisk);
  cr_bitmap(1, 0);

}
