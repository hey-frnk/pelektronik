#include <stdint.h>
#include "MP3Display.hpp"

#ifndef TrackDISPLAY_HPP
#define TrackDISPLAY_HPP

class TrackDisplay : public MP3Display{

private:
  uint8_t      volume, mode, status, *bufLength;
  char         *trackName, *artistName, *albumName;
  uint32_t     length;

public:
  uint8_t      getType();
  void         show();
  void         setTrackInfo(char*, char*, char*, uint32_t);
  void         changeVolume(uint8_t);
  void         changeMode(uint8_t);
  void         changeStatus(uint8_t);

};


#endif
