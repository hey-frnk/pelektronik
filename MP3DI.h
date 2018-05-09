// MP3 Data Interface functions

typedef struct Track{
  char* trackName;
  char* artistName;
  char* albumname;
  uint32_t length;
} Track;

typedef struct TrackList{
  Track* track;
  struct strList* next;
} TrackList;


// This functions requests a list of tracks in a linked list
TrackList* MP3DI_requestTrackList(void);


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
