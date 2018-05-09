#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MP3Display.h"
#include "TrackDisplay.h"
#include "TrackEQDisplay.h"

TrackDisplay* INSTANCE_TrackDISPLAY = NULL;
TrackEQDisplay* INSTANCE_TrackEQDISPLAY = NULL;

int main() {
  MP3Display *m[2];

  // Initialize
  INSTANCE_TrackDISPLAY = malloc(sizeof(TrackDisplay));
  INSTANCE_TrackEQDISPLAY = malloc(sizeof(TrackEQDisplay));

  TrackDisplay_init(INSTANCE_TrackDISPLAY);
  TrackEQDisplay_init(INSTANCE_TrackEQDISPLAY);

  INSTANCE_TrackDISPLAY->changeVolume(INSTANCE_TrackDISPLAY, 18);
  INSTANCE_TrackEQDISPLAY->super.setBatteryState(&(INSTANCE_TrackEQDISPLAY->super), 9);

  m[0] = (MP3Display *)(INSTANCE_TrackDISPLAY);
  m[1] = (MP3Display *)(INSTANCE_TrackEQDISPLAY);

  for(int i = 0; i < 2; ++i){
    m[i]->show(m[i]);
    m[i]->setBatteryState(m[i], 99);
    time t = {12 + i, 13 + i, 14 - i};
    m[i]->updateTime(m[i], t);
  }
}
