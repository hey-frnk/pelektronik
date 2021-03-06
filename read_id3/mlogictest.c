#include <stdio.h>
#include <stdlib.h>
#include "read_mheader.h"

// gcc mlogictest.c read_mheader.c -std=c11 -Wall -o mlogictest

int main(){
  char* path = "Crystal Lights.mp3";

  FILE* file;
  file = fopen(path, "r");

  // unsigned char* buffer = malloc(fileSize * sizeof(unsigned char));
  // fread(buffer, 1, fileSize, file);

  // mheader h;
  uint32_t duration = mheader_getLength(file);
  printf("Song Duration: %hhu:%hhu\n", (uint8_t)(duration / 60), (uint8_t)(duration % 60));

  /*uint32_t firstFrameOffset = mheader_getFirstFrameOffset(buffer, fileSize);

  mheader h;
  mheader_init(&h, buffer + firstFrameOffset);

  printf("Frame Size: %u\n", mheader_getFrameSize(&h));
  // printf("Frame Count: %u\n", mheader_getTotalFrameCount(buffer + firstFrameOffset));
  uint32_t duration = mheader_getLength(&h, mheader_getTotalFrameCount(buffer + firstFrameOffset));
  printf("Song Duration: %hhu, %hhu\n", (uint8_t)(duration / 60), (uint8_t)(duration % 60));*/

}
