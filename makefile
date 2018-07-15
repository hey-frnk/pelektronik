main: maindebug.o MP3Display.o MP3Display_Message.o TrackDisplay.o RecordDisplay.o MP3Display_routines.o MenuDisplay.o mdisplay_color.o mdisplay_hlvf.o himage.o MP3DI.o MP3BI.o SDI.o RTCI.o read_id3.o read_bmp.o read_mheader.o
	clang++ -DDEBUG -o main maindebug.o MP3Display.o MP3Display_Message.o TrackDisplay.o MenuDisplay.o RecordDisplay.o mdisplay_color.o mdisplay_hlvf.o MP3Display_routines.o himage.o MP3DI.o MP3BI.o SDI.o RTCI.o read_id3.o read_bmp.o read_mheader.o

maindebug.o: maindebug.cpp
	clang++ -x c++ maindebug.cpp -DDEBUG -std=c++11 -Wall -c

MP3Display.o: MP3Display.c
	clang -x c MP3Display.c -DDEBUG -std=c11 -Wall -c

MP3Display_Message.o: MP3Display_Message.c
	clang -x c MP3Display_Message.c -DDEBUG -std=c11 -Wall -c

TrackDisplay.o: TrackDisplay.c
	clang -x c TrackDisplay.c -DDEBUG -std=c11 -Wall -c

MenuDisplay.o: MenuDisplay.c
	clang -x c MenuDisplay.c -DDEBUG -std=c11 -Wall -c

RecordDisplay.o: RecordDisplay.c
	clang -x c RecordDisplay.c -DDEBUG -std=c11 -Wall -c

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

MP3BI.o: MP3BI.c
	clang -x c MP3BI.c -DDEBUG -std=c11 -Wall -c

SDI.o: SDI.c
	clang -x c SDI.c -DDEBUG -std=c11 -Wall -c

RTCI.o: RTCI.c
	clang -x c RTCI.c -DDEBUG -std=c11 -Wall -c

read_id3.o: read_id3/read_id3.c
	clang -x c read_id3/read_id3.c -DDEBUG -std=c11 -Wall -c

read_bmp.o: read_id3/read_bmp.c
	clang++ -x c++ read_id3/read_bmp.c -DDEBUG -std=c++11 -Wall -c

read_mheader.o: read_id3/read_mheader.c
	clang -x c read_id3/read_mheader.c -DDEBUG -std=c11 -Wall -c
