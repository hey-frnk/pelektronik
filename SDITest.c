#include <stdio.h>
#include "SDI.h"
#include "MP3DI.h"

// gcc sdi.c sditest.c mp3di.c -DDEBUG -std=c11 -Wall -o sditest

const char* types[4] = {"Directory", "MP3 File", "Bitmap Image", "No Freaking Idea"};

int main(){

  printf("***************** LISTING FILES *****************\n");
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

  return 0;
}