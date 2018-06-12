#include <stdio.h>
#include "SDI.h"

int main(){
  SD_FILE_LIST *l = SDI_getFileListFromDirectory("Irrelefant!");

  printf("List Size: %d\n", l->FILE_LIST_SIZE);

  for(int i = 0; i < l->FILE_LIST_SIZE; ++i)
    printf("File Name: %s, File Size: %d\n", l->FILE_LIST[i]->SD_FILE_NAME, l->FILE_LIST[i]->SD_FILE_SIZE);

  return 0;
}
