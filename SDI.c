#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "SDI.h"


// Get file ending
static uint8_t _SDI_retrieveExtension(char *fileName){
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
            	char *fName = malloc(strlen(fno.fname) * sizeof(char));
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
  // Create some random files in a stupid way
  SD_FILE* file1 = malloc(sizeof(SD_FILE));
  file1->SD_FILE_NAME = "paradise.mp3";
  file1->SD_FILE_TYPE = _SDI_retrieveExtension(file1->SD_FILE_NAME);
  file1->SD_FILE_SIZE = 1234567; // (1.23 MB)

  SD_FILE* file2 = malloc(sizeof(SD_FILE));
  file2->SD_FILE_NAME = "coastline.mp3";
  file2->SD_FILE_TYPE = _SDI_retrieveExtension(file2->SD_FILE_NAME);
  file2->SD_FILE_SIZE = 2345678; // (2.35 MB)

  SD_FILE* file3 = malloc(sizeof(SD_FILE));
  file3->SD_FILE_NAME = "trumptower.bmp";
  file3->SD_FILE_TYPE = _SDI_retrieveExtension(file3->SD_FILE_NAME);
  file3->SD_FILE_SIZE = 999999; // (1.00 MB)

  SD_FILE* file4 = malloc(sizeof(SD_FILE));
  file4->SD_FILE_NAME = "somecode.cpp";
  file4->SD_FILE_TYPE = _SDI_retrieveExtension(file4->SD_FILE_NAME);
  file4->SD_FILE_SIZE = 5555; // (0.06 MB)

  SD_FILE* file5 = malloc(sizeof(SD_FILE));
  file5->SD_FILE_NAME = "whiteblood.mp3";
  file5->SD_FILE_TYPE = _SDI_retrieveExtension(file5->SD_FILE_NAME);
  file5->SD_FILE_SIZE = 9837837; // (9.84 MB)

  SD_FILE* file6 = malloc(sizeof(SD_FILE));
  file6->SD_FILE_NAME = "All I Want.mp3";
  file6->SD_FILE_TYPE = _SDI_retrieveExtension(file6->SD_FILE_NAME);
  file6->SD_FILE_SIZE = 765432;

  SD_FILE* file7 = malloc(sizeof(SD_FILE));
  file7->SD_FILE_NAME = "DearDiary.txt";
  file7->SD_FILE_TYPE = _SDI_retrieveExtension(file7->SD_FILE_NAME);
  file7->SD_FILE_SIZE = 3344;

  SDI_push(newList, file1);
  SDI_push(newList, file2);
  SDI_push(newList, file3);
  SDI_push(newList, file4);
  SDI_push(newList, file5);
  SDI_push(newList, file6);
  SDI_push(newList, file7);
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
  for(uint32_t i = 0; i < list->FILE_LIST_SIZE; ++i) free(list->FILE_LIST[i]);
  free(list->FILE_LIST);
  free(list);
}
