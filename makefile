main: maindebug.o MP3Display.o TrackDisplay.o MP3Display_routines.o MenuDisplay.o mdisplay_color.o mdisplay_hlvf.o himage.o MP3DI.o SDI.o
	clang++ -DDEBUG -o main maindebug.o MP3Display.o TrackDisplay.o MenuDisplay.o mdisplay_color.o mdisplay_hlvf.o MP3Display_routines.o himage.o MP3DI.o SDI.o

maindebug.o: maindebug.cpp
	clang++ -x c++ maindebug.cpp -DDEBUG -std=c++11 -Wall -c

MP3Display.o: MP3Display.c
	clang -x c MP3Display.c -DDEBUG -std=c11 -Wall -c

TrackDisplay.o: TrackDisplay.c
	clang -x c TrackDisplay.c -DDEBUG -std=c11 -Wall -c

MenuDisplay.o: MenuDisplay.c
	clang -x c MenuDisplay.c -DDEBUG -std=c11 -Wall -c

MP3Display_routines.o: MP3Display_routines.c
	clang++ -x c++ MP3Display_routines.c -DDEBUG -std=c++11 -Wall -c

mdisplay_color.o: mdisplay_color.c
	clang -x c mdisplay_color.c -DDEBUG -std=c11 -Wall -c

mdisplay_hlvf.o: mdisplay_hlvf.c
	clang++ -x c++ mdisplay_hlvf.c -DDEBUG -std=c++11 -Wall -c

himage.o: debug/himage.cpp
	clang++ debug/himage.cpp -DDEBUG -std=c++11 -Wall -c

MP3DI.o: MP3DI.c
	clang -x c MP3DI.c -DDEBUG -std=c11 -Wall -c

SDI.o: SDI.c
	clang -x c SDI.c -DDEBUG -std=c11 -Wall -c