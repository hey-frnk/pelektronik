/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

// SD Card Data Interface functions

#ifndef SDI_H
#define SDI_H

#include <stdint.h>

enum SD_FILE_TYPES {
  TYPE_DIRECTORY,
  TYPE_MP3FILE,
  TYPE_BMPIMAGE,
  TYPE_UNKNOWN
};

typedef struct SD_FILE {
  uint8_t           SD_FILE_TYPE;     // Should be within SD_FILE_TYPES.
  char              *SD_FILE_NAME;    // File name
  uint32_t          SD_FILE_SIZE;     // Every file should have a valid size
} SD_FILE;

typedef struct SD_FILE_LIST {
  SD_FILE           **FILE_LIST;      // List of files (array)
  uint32_t          FILE_LIST_SIZE;   // Array size
} SD_FILE_LIST;

#ifndef DEBUG
void 			SDI_Init();
#endif

// Allocates new list with all files
SD_FILE_LIST*   SDI_getFileListFromDirectory(char *dir);

// Add new file
void            SDI_push(SD_FILE_LIST *list, SD_FILE *file);

// Free the allocated memory
void            SDI_free(SD_FILE_LIST *list);



#endif

#ifdef __cplusplus
}
#endif
