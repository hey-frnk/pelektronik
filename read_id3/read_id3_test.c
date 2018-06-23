#include <stdio.h>
#include <stdlib.h>
#include "read_id3.h"
#include "../MP3DI.h"

// gcc read_id3_test.c read_id3.c read_mheader.c -DDEBUG -std=c11 -Wall -o read_id3_test

// const char* types[4] = {"Directory", "MP3 File", "Bitmap Image", "No Freaking Idea"};

int main(){

  char* file_name = "01 Here We Are.mp3";

  FIL* file;
  file = fopen(file_name, "r");
  char str[128];
  read_ID3_info(TITLE_ID3, str, sizeof(str), file);
  printf("Title: %s\n", str);
  for(uint8_t i = 0; i < 16; ++i) *(((uint64_t *)str) + i) = 0;

  read_ID3_info(ALBUM_ID3, str, sizeof(str), file);
  printf("Album: %s\n", str);
  for(uint8_t i = 0; i < 16; ++i) *(((uint64_t *)str) + i) = 0;

  read_ID3_info(ARTIST_ID3, str, sizeof(str), file);
  printf("Artist: %s\n", str);
  for(uint8_t i = 0; i < 16; ++i) *(((uint64_t *)str) + i) = 0;

  char str2[10] = {0};
  read_ID3_info(LENGTH_ID3, str2, sizeof(str2), file);
  printf("Length: %s\n", str2);

  fclose(file);

  /*printf("***************** LISTING FILES *****************\n");
  SD_FILE_LIST *l = SDI_getFileListFromDirectory("Irrelefant!");
  printf("List Size: %d\n", l->FILE_LIST_SIZE);

  for(int i = 0; i < l->FILE_LIST_SIZE; ++i)
    printf("File Name: %s, Type: %s, File Size: %d\n", l->FILE_LIST[i]->SD_FILE_NAME, types[l->FILE_LIST[i]->SD_FILE_TYPE], l->FILE_LIST[i]->SD_FILE_SIZE);


  printf("****************** LISTING MP3 ******************\n");
  TrackList *tl = MP3DI_initTrackListFromFileList(l);

  SDI_free(l);
  printf("File List Free'd\n");

  printf("MP3 Track List Size: %d\n", tl->size);

  for(int i = 0; i < tl->size; ++i){
    Track *rTrack = MP3DI_TrackList_retrieveTrack(tl, i);
    printf("Name: %s, Track Name: %s, ", rTrack->fileName, rTrack->trackName);
    printf("Artist: %s, Album: %s, Length: %d\n", rTrack->artistName, rTrack->albumName, rTrack->length);
  }

  MP3DI_TrackList_free(tl);
  printf("Track List Free'd\n");

  return 0;*/
}
