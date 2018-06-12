#ifdef __cplusplus
extern "C" {
#endif

// MP3 Data Interface functions

#ifndef MP3DI_H
#define MP3DI_H

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


// This functions returns an empty track list
TrackList* MP3DI_initTrackList(void);

// Add new song to track list
void MP3DI_TrackList_add(TrackList* list, Track *track);

// Clear track list
void MP3DI_TrackList_clear(TrackList *list);

// Retrieve track
Track* MP3DI_TrackList_retrieveTrack(TrackList *list, uint32_t pos);


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
