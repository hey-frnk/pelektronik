#include <stdlib.h>
#include "MP3DI.h"

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

      #ifdef DEBUG
      if(i & 1) newTrack->trackName = "Technicolour Beat";
      else newTrack->trackName = "Lose It";
      newTrack->artistName = "Oh Wonder";
      newTrack->albumName = "Oh Wonder [Deluxe]";
      #endif

      newTrack->length = (uint32_t)(list->FILE_LIST[i]->SD_FILE_SIZE / 2.0f);
      trackList->list[_track_count] = newTrack;
      ++_track_count;
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
