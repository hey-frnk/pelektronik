#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MP3DI.h"
#include "read_id3/read_id3.h"

#define BUFSIZE 128

TrackList* __attribute__((weak)) MP3DI_initTrackListFromFileList(SD_FILE_LIST *list){
  // Get number of songs
  uint32_t _track_count = 0;
  for(uint32_t i = 0; i < list->FILE_LIST_SIZE; ++i) if(list->FILE_LIST[i]->SD_FILE_TYPE == TYPE_MP3FILE) _track_count++;

  // New track list instance with the count of songs in this directory
  TrackList *trackList = (TrackList *)malloc(sizeof(TrackList));
  trackList->size = _track_count;

  trackList->list = (Track **)malloc(trackList->size * sizeof(Track *));

  // Fill in new tracks, reset counter variable
  _track_count = 0;
  for(uint32_t i = 0; i < list->FILE_LIST_SIZE; ++i){
    // If MP3, push item into array
    if(list->FILE_LIST[i]->SD_FILE_TYPE == TYPE_MP3FILE){
      Track* newTrack = (Track *)malloc(sizeof(Track));
      newTrack->fileName = list->FILE_LIST[i]->SD_FILE_NAME;

      FIL* file;
      #ifdef DEBUG
      file = fopen(newTrack->fileName, "r");
      #else
      f_open(file, newTrack->fileName, FA_READ);
      #endif

      // read title, album and artist
      char *buf0 = (char *)calloc(BUFSIZE, sizeof(char));
      read_ID3_info(TITLE_ID3, buf0, BUFSIZE, file);
      newTrack->trackName = (char *)calloc(strlen(buf0) + 1, sizeof(char));
      strcpy(newTrack->trackName, buf0);
      free(buf0);

      char *buf1 = (char *)calloc(BUFSIZE, sizeof(char));
      read_ID3_info(ALBUM_ID3, buf1, BUFSIZE, file);
      newTrack->albumName = (char *)calloc(strlen(buf1) + 1, sizeof(char));
      strcpy(newTrack->albumName, buf1);
      free(buf1);

      char *buf2 = (char *)calloc(BUFSIZE, sizeof(char));
      read_ID3_info(ARTIST_ID3, buf2, BUFSIZE, file);
      newTrack->artistName = (char *)calloc(strlen(buf2) + 1, sizeof(char));
      strcpy(newTrack->artistName, buf2);
      free(buf2);

      char* buf3 = (char *)calloc(BUFSIZE, sizeof(char));
      read_ID3_info(LENGTH_ID3, buf3, BUFSIZE, file);
      newTrack->length = atoi(buf3);
      free(buf3);

      trackList->list[_track_count] = newTrack;
      ++_track_count;

      #ifdef DEBUG
      fclose(file);
      #else
      f_close(file);
      #endif
    }
  }

  return trackList;
}

Track* MP3DI_TrackList_retrieveTrack(TrackList *list, uint32_t pos){
  if(pos >= list->size) return NULL;
  return list->list[pos];
}

void MP3DI_TrackList_free(TrackList *list){
  for(uint32_t i = 0; i < list->size; ++i) free(list->list[i]);
  free(list->list);
  free(list);
}









// Hello
