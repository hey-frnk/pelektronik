/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

// MP3 Data Interface functions
// Why do assembly programmers need to know how to swim? ...

#ifndef MP3DI_H
#define MP3DI_H

#include <stdint.h>
#include "SDI.h"

typedef struct Track{       // A track has
  char *fileName;           // a file name
  char *trackName;          // a song track name
  char *artistName;         // an artist
  char *albumName;          // and belongs to an album
  uint32_t length;          // length is the total track length in seconds
} Track;

typedef struct TrackList{   // A track list is an array of tracks
  Track **list;             // This is the array itself
  uint32_t size;            // The size tells how many items are in here
} TrackList;


// This functions returns a song list upon a given file list
TrackList* MP3DI_initTrackListFromFileList(SD_FILE_LIST *list);

// Free track list
void MP3DI_TrackList_free(TrackList *list);

// Free a single track
void MP3DI_TrackFree(Track *track);

// Retrieve track from file name
Track* MP3DI_retreiveTrackFromFileName(const char *fileName);

// Retrieve track
Track* MP3DI_retrieveTrackFromTrackList(TrackList *l, uint32_t pos);

// Retrieve track length
void MP3DI_retrieveTrackLength(Track *t);


// Play Track
// When this function is called, a Track track is played
// Should return 0 if successful
uint8_t MP3DI_playTrack(Track *track);


// Pause Track
void MP3DI_pauseTrack(void);


// Resume Track
void MP3DI_resumeTrack(void);


// Skip Forward
// Should return 0 if successful
uint8_t MP3DI_skipForward(void);


// Skip Forward
// Should return 0 if successful
uint8_t MP3DI_skipBackward(void);


#endif

#ifdef __cplusplus
}
#endif





// ...Because they work below C level.      #goVegan #dadProgrammerJokes
