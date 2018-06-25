/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "SDI.h"

// Why did the pirates code fail to compile?

char *_concatSDI(const char *s1, const char *s2){
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

#ifdef DEBUG
  #define SAMPLE_FILES 9

  const char* tmp[SAMPLE_FILES] = {
    "read_id3/Mark Diamond - Find You.mp3",
    "read_id3/04. Love Alone.mp3",
    "read_id3/22 (feat. Sam Tsui, Kurt Schneider, Against the Current and King the Kid).mp3",
    "read_id3/Rival x Cadmium - Seasons (feat. Harley Bird).mp3",
    "read_id3/DoubleV - Indigo.mp3",
    "read_id3/Autograf – Sleepless In NYC.mp3",
    "read_id3/The Tech Thieves  - Work On Me ~by KnightVision [soundtake.net].mp3",
    "read_id3/THBD - Lost In The Night (Feat. Pipa Moran).mp3",
    "read_id3/faded_conor_maynard.mp3"
  };
#endif

// Get file ending
static uint8_t _SDI_retrieveExtension(const char *fileName){
  char *_extension = strrchr(fileName, '.');
  if(!_extension || _extension == fileName) return TYPE_DIRECTORY;
  ++_extension;

  if(!strcmp(_extension, "MP3")) return TYPE_MP3FILE;
  else if(!strcmp(_extension, "BMP")) return TYPE_BMPIMAGE;
  else if(!strcmp(_extension, "mp3")) return TYPE_MP3FILE;
  else if(!strcmp(_extension, "bmp")) return TYPE_BMPIMAGE;
  return TYPE_UNKNOWN;
}


#ifndef DEBUG

/*#define FF_USE_LFN 1
#define FF_MAX_LFN 128
#define FF_LFN_UNICODE 0
#define FF_LFN_BUF 0
#define FF_STRF_ENCODE 0*/


#include "stm32_adafruit_sd.h"
#include "ffs/ff_gen_drv.h"
#include "ffs/drivers/sd_diskio.h"

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

void SDI_Init() {
	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		/*##-2- Register the file system object to the FatFs module ##############*/
		// volatile FRESULT r = f_mount(&SDFatFs, (TCHAR const*)SDPath, 0);
		volatile FRESULT r = f_mount(&SDFatFs, "", 1);
		if(r != FR_OK); // Error Handler -> !!!!!!!!!!!!!!!!!!!!!!

		/*
		else {
			// ##-3- Create a FAT file system (format) on the logical drive (was nicht passend ist wird passend gemacht? ^^ #########
			// WARNING: Formatting the uSD card will delete all content on the device
			if(f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK); // Error Handler -> !!!!!!!!!!!!!!!!!!!!!!!!
			else {
				// ##-4- Create and Open a new text file object with write access #####
				if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK); // ERROR HANDLER
				else {
					// ##-5- Write data to the text file ################################
					res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

					// ##-6- Close the open text file #################################
					if (f_close(&MyFile) != FR_OK); // ERROR HANDLER
					if((byteswritten == 0) || (res != FR_OK)); // ERROR HANDLER
					else {
						// ##-7- Open the text file object with read access ###############
						if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK); // ERROR HANDLER
						else {
							// ##-8- Read data from the text file ###########################
							res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
							if((bytesread == 0) || (res != FR_OK)); // ERROR HANDLER
							else  f_close(&MyFile);
						}
					}
				}
			}
		}*/
	}
	/*##-11- Unlink the RAM disk I/O driver ####################################*/
	// FATFS_UnLinkDriver(SDPath);
}

FRESULT scan_files (char* path, SD_FILE_LIST *lInstance) {
    volatile FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    // Directory
                /*i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path, lInstance);                    // Enter
                if (res != FR_OK) break;
                path[i] = 0;*/
            } else {                 						// File
            	if(fno.fname[0] == '_') continue; 			// Track the beautiful files only
                // printf("%s/%s\n", path, fno.fname);
            	SD_FILE *f1 = malloc(sizeof(SD_FILE)); 		// Create a new file link

            	//char *fNamePath = _concatSDI("/", path);
            	//char *fName = _concatSDI(fNamePath, fno.fname);
            	//free(fNamePath);

            	char *fName = malloc(strlen(fno.fname) * sizeof(char) + 1);
            	strcpy(fName, fno.fname);

            	f1->SD_FILE_TYPE = _SDI_retrieveExtension(fno.fname);
            	f1->SD_FILE_NAME = fName;
            	f1->SD_FILE_SIZE = fno.fsize;
            	SDI_push(lInstance, f1);
            }
        }
        f_closedir(&dir);
    }

    return res;
}


#endif

// Allocates new list with all files
SD_FILE_LIST* SDI_getFileListFromDirectory(char *dir){

  // Create new list
  SD_FILE_LIST *newList = (SD_FILE_LIST *)calloc(1, sizeof(SD_FILE_LIST));

  #ifndef DEBUG
  scan_files("", newList);

  #else

  for(int i = 0; i < SAMPLE_FILES; ++i) {
    SD_FILE *file = malloc(sizeof(SD_FILE));
    file->SD_FILE_NAME = (char *)calloc(strlen(tmp[i]) + 1, sizeof(char));
    strcpy(file->SD_FILE_NAME, tmp[i]);
    file->SD_FILE_TYPE = _SDI_retrieveExtension(file->SD_FILE_NAME);
    file->SD_FILE_SIZE = i * 512;
    SDI_push(newList, file);
  }
  #endif

  return newList;

}

void SDI_push(SD_FILE_LIST *list, SD_FILE *file){
  if(list->FILE_LIST == NULL) {  // Empty List
    ++list->FILE_LIST_SIZE;
    list->FILE_LIST = (SD_FILE **)calloc(list->FILE_LIST_SIZE, sizeof(SD_FILE *));
    list->FILE_LIST[0] = file;
  } else {
    // Create new list and fill with current list elements. Replace with current list
    SD_FILE **newList = (SD_FILE **)calloc(++list->FILE_LIST_SIZE, sizeof(SD_FILE *));
    uint32_t i;
    for(i = 0; i < list->FILE_LIST_SIZE - 1; ++i) newList[i] = list->FILE_LIST[i];
    free(list->FILE_LIST);
    newList[i] = file;
    list->FILE_LIST = newList;
  }
}

// Free the allocated memory
void SDI_free(SD_FILE_LIST *list){
  for(uint32_t i = 0; i < list->FILE_LIST_SIZE; ++i){
    free(list->FILE_LIST[i]->SD_FILE_NAME);
    free(list->FILE_LIST[i]);
  }
  free(list->FILE_LIST);
  free(list);
}




// They forgot the arrrrrrrrguments. #goVegan
