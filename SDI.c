#include <stdlib.h>
#include <stdint.h>
#include "SDI.h"

// Allocates new list with all files
SD_FILE_LIST* SDI_getFileListFromDirectory(char *dir){

  // Create new list
  SD_FILE_LIST *newList = (SD_FILE_LIST *)malloc(sizeof(SD_FILE_LIST));

  #ifdef DEBUG
  // Create some random files in a stupid way
  SD_FILE* file1 = malloc(sizeof(SD_FILE));
  file1->SD_FILE_TYPE = TYPE_MP3FILE;
  file1->SD_FILE_NAME = "paradise.mp3";
  file1->SD_FILE_SIZE = 1234567; // (1.23 MB)

  SD_FILE* file2 = malloc(sizeof(SD_FILE));
  file2->SD_FILE_TYPE = TYPE_MP3FILE;
  file2->SD_FILE_NAME = "coastline.mp3";
  file2->SD_FILE_SIZE = 2345678; // (2.35 MB)

  SD_FILE* file3 = malloc(sizeof(SD_FILE));
  file3->SD_FILE_TYPE = TYPE_BMPIMAGE;
  file3->SD_FILE_NAME = "trumptower.bmp";
  file3->SD_FILE_SIZE = 999999; // (1.00 MB)

  SD_FILE* file4 = malloc(sizeof(SD_FILE));
  file4->SD_FILE_TYPE = TYPE_UNKNOWN;
  file4->SD_FILE_NAME = "somecode.cpp";
  file4->SD_FILE_SIZE = 5555; // (0.06 MB)

  // Create new list
  uint32_t _size = 4;
  newList->FILE_LIST_SIZE = _size;
  newList->FILE_LIST = (SD_FILE **)malloc(newList->FILE_LIST_SIZE * sizeof(SD_FILE *));

  newList->FILE_LIST[0] = file1;
  newList->FILE_LIST[1] = file2;
  newList->FILE_LIST[2] = file3;
  newList->FILE_LIST[3] = file4;
  #endif

  return newList;

}

// Free the allocated memory
void SDI_free(SD_FILE_LIST *list){
  // Do nothing.
}
