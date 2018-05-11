main: MP3Display.c TrackDisplay.c TrackEQDisplay.c MP3Display_routines.c main.c
	gcc MP3Display.c TrackDisplay.c TrackEQDisplay.c MP3Display_routines.c main.c -std=c11 -Wall -o main
