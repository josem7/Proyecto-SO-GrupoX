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
  cr_ls(1);
  int result = cr_exists(3, "guides.txt");
  printf("%d\n", result);
  cr_hardlink(1, "text.txt", "porfa_funciona.txt");
  cr_ls(1);
}
