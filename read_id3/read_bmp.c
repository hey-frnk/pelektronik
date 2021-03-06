#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_bmp.h"

#ifdef DEBUG
#include "../mdisplay_color.h"
#include "../mdisplay_hlvf.h"
#else
#include "../Inc/mdisplay_color.h"
#include "../Inc/mdisplay_hlvf.h"
#include "../Inc/st7735.h"
#endif

#ifdef __arm__  // for an embedded enviroment, using FatFs from chan
  #include "../Middlewares/Third_Party/FatFs/src/ff.h"				// FAT File System Library
  #include "../Middlewares/Third_Party/FatFs/src/diskio.h" 			// Disk IO Initialize SPI Mutex
  #define file_seek_absolute(file,position) f_lseek(file, position)
  #define file_seek_relative(fi,pos) f_lseek(fi,fi->fptr+pos)
  #define file_read(f,str,l,rea) f_read(f,str,(l),&(rea))
#else
  #define file_seek_absolute(file,position) fseek (file , position , SEEK_SET)
  #define file_seek_relative(fi,pos) fseek(fi,pos,SEEK_CUR)
  #define file_read(f,str,l,rea) rea=fread(str,1,l,f)
  #define FIL FILE
  #define rprintf printf
#endif

void readbmp_DrawBitmapFromFile(const char *fileName) {
  uint32_t width, height, offset, bytesread;
  uint8_t header[0x36];

  #ifdef DEBUG
  FIL *file;
  file = fopen(fileName, "r");
  #else
  FIL file;
  volatile FRESULT result = f_open(&file, fileName, FA_READ);
  #endif

  #ifdef DEBUG
  file_read(file, header, sizeof(header), bytesread);
  #else
  file_read(&file, header, sizeof(header), bytesread);
  #endif

  offset = *((uint32_t *)&header[0x0A]);
  width = *((uint32_t *)&header[0x12]);
  height = *((uint32_t *)&header[0x16]);

  #ifdef DEBUG
  file_seek_absolute(file, offset);
  #else
  file_seek_absolute(&file, offset);
  #endif

  for(uint8_t y = 0; y < height; ++y) {

    for(uint8_t x = 0; x < width; x += 2) {
      uint8_t bgrTmp[6];
      #ifdef DEBUG
      file_read(file, &bgrTmp, 6, bytesread);
      uint16_t color1 = mdisplay_rgb_to565(bgrTmp[2], bgrTmp[1], bgrTmp[0]);
      uint16_t color2 = mdisplay_rgb_to565(bgrTmp[5], bgrTmp[4], bgrTmp[3]);
      mdisplay_hlvf_DrawRectangle(x, height - y - 1, 1, 1, color1);
      mdisplay_hlvf_DrawRectangle(x + 1, height - y - 1, 1, 1, color2);
      #else
      file_read(&file, &bgrTmp, 6, bytesread);
      uint16_t color1 = mdisplay_rgb_to565(bgrTmp[2], bgrTmp[1], bgrTmp[0]);
      uint16_t color2 = mdisplay_rgb_to565(bgrTmp[5], bgrTmp[4], bgrTmp[3]);
      st7735_WritePixel(x, height - y - 1, color1);
      st7735_WritePixel(x + 1, height - y - 1, color2);
      #endif
    }
  }

  #ifdef DEBUG
  fclose(file);
  #else
  f_close(&file);
  #endif
}

//
