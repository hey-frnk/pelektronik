/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include <stdio.h>
#include "SDI.h"
#include "MP3DI.h"

// gcc sdi.c sditest.c mp3di.c read_id3/read_id3.c read_id3/read_mheader.c -DDEBUG -DKATORDGABIGAGAGAGAGA -std=c11 -Wall -o sditest

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
    printf("\n+++++ MP3 File Start +++++\n");
    printf("Name: %s\n", rTrack->fileName);
    printf("Track Name: %s\n", rTrack->trackName);
    printf("Artist: %s\n", rTrack->artistName);
    printf("Album: %s\n", rTrack->albumName);
    printf("Length: %d\n", rTrack->length);
    printf("++++++ MP3 File End ++++++\n");
  }

  MP3DI_TrackList_free(tl);
  printf("Track List Free'd\n");

  return 0;
}
