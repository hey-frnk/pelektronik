//
//  himage.cpp
//  himage
//
//  Copyright © 2017 Frank Zheng. All rights reserved.
//

#include <string>
#include <unistd.h>
#include "himage.h"
#include "../mdisplay_color.h"

hImage::hImage(uint32_t width, uint32_t height, std::string fileName) :
width(width), height(height), fileName(fileName){
  imageData = new uint8_t[3 * width * height];
}

hImage::hImage(uint32_t width, uint32_t height) :
width(width), height(height){
  imageData = new uint8_t[3 * width * height];
}

hImage::hImage(std::string path) :
fileName(path)
{
  hBMPContainer* picture1 = new hBMPContainer(path);
  width = picture1 -> getWidth();
  height = picture1 -> getHeight();
  imageData = picture1 -> read();
  delete picture1;
}

hImage::~hImage(){
  delete[] imageData;
}

uint32_t hImage::getWidth(){
  return width;
}

uint32_t hImage::getHeight(){
  return height;
}

uint8_t hImage::getPixel(uint32_t x, uint32_t y, uint8_t colorOffset){
  return imageData[(3 * x * width) + (3 * y) + colorOffset];
}

void hImage::toBitStream(){
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < height; ++j){
      uint8_t cV = (uint8_t)((getPixel(i, j, RED) + getPixel(i, j, GREEN) + getPixel(i, j, BLUE)) / 3.0);
      printf("0x%hhx, ", cV);
      /*if(cV == 0xFF) printf(" ");
      else if (cV == 0x75) printf("@");
      else printf("+");*/
    }
    printf("\n");
  }
}

void hImage::to565(){
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < height; ++j){
      uint16_t cV = (uint16_t)(   ((getPixel(i, j, RED) & 0b11111000) << 8)     |     ((getPixel(i, j, GREEN) & 0b11111100) << 3)     |     ((getPixel(i, j, BLUE)) >> 3)   );
      printf("0x%hx, ", cV);
    }
    printf("\n");
  }
}

void hImage::setPixel(uint32_t x, uint32_t y, uint8_t value, uint8_t colorOffset){
  imageData[(3 * x * width) + (3 * y) + colorOffset] = value;
}

void hImage::setPixelRGB(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b){
  setPixel(x, y, g, GREEN);
  setPixel(x, y, r, RED);
  setPixel(x, y, b, BLUE);
}

void hImage::setBWPixel(uint32_t x, uint32_t y, uint8_t value){
  setPixel(x, y, value, GREEN);
  setPixel(x, y, value, RED);
  setPixel(x, y, value, BLUE);
}

void hImage::setRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t value, uint8_t color){
  for(int i = 0; i < w; i++) for(int j = 0; j < h; j++) setPixel(x + i, y + j, value, color);
}

void hImage::setFileName(std::string fileName){
  this -> fileName = fileName;
}

void hImage::save(){
  hBMPContainer* picture1 = new hBMPContainer(imageData, width, height);
  picture1->compile();
  picture1->render(fileName);
  delete picture1;
}

void hImage::save(std::string name){
  setFileName(name);
  save();
}

std::string hImage::saveAndReturnPath(){
  hBMPContainer* picture1 = new hBMPContainer(imageData, width, height);
  picture1->compile();
  std::string r = picture1->renderAndReturnPath(fileName);
  delete picture1;
  return r;
}

std::string hImage::saveAndReturnPath(std::string name){
  setFileName(name);
  return saveAndReturnPath();
}

// Internal HBMP Functions

#define MAXPATHLEN 1024

std::string get_working_path()
{
   char temp[MAXPATHLEN];
   return (getcwd(temp, MAXPATHLEN) ? std::string(temp) : std::string(""));
}

/* Write initializer.
 - Copy bgrArr into compilable BMP Format
 - Set all BMP parameters
*/
hBMPContainer::hBMPContainer(uint8_t* bgrArr, uint32_t width, uint32_t height){
  hasCompiled = false;

  uint32_t widthAlign = 3 * width;

  // Calculate padding Fill

  uint32_t paddingFills = 0;
  if((widthAlign) % 4 != 0) paddingFills = 4 - ((widthAlign) % 4);

  // Calculate new array size
  uint32_t arrHSize = (paddingFills + widthAlign);
  uint32_t arrSize = arrHSize * height;

  // Allocate BMP Data
  bmpData = new uint8_t[arrSize];

  uint32_t iVOffset = arrSize - arrHSize;
  uint32_t iArrOffset = 0;

  // Go along each row
  for(uint32_t row = 0; row < height; row++){

      // Fill the 3 * width columns with bgr data
      for(uint32_t i = 0; i < widthAlign; i++) bmpData[iVOffset + i] = bgrArr[iArrOffset + i];
      // And the remaining columns with zero padding
      for(uint32_t i = widthAlign; i < (widthAlign + paddingFills); i++) bmpData[iVOffset + i] = 0x00;

      // Offset Increase
      iVOffset -= arrHSize;
      iArrOffset += widthAlign;
  }

  // File Header

  h_bmpType1 = 0x42;
  h_bmpType2 = 0x4D;

  h_reserved1 = 0;
  h_reserved2 = 0;

  h_infoOffset = 54;


  // Info Header

  i_SizeHeader = 40;
  i_Width = width;
  i_Height = height;
  i_Planes = 1;
  i_BitCount = 24;
  i_Compression = 0;

  // +++++++++++++++

  i_XPelsPerMeter = 2835;
  i_YPelsPerMeter = 2835;
  i_ClrUsed = 0;
  i_ClrImportant = 0;


  // Size calculation
  i_SizeImage = arrSize;
  h_fileSize = arrSize + 54;
}

/* Read initializer.
 - Read all BMP parameters
*/
hBMPContainer::hBMPContainer(std::string path){
  hasCompiled = false;

  char cPath[path.length() + 1];
  std::strcpy(cPath, path.c_str());
  FILE *bmpFile;
  bmpFile = fopen(cPath, "r");

  fseek(bmpFile, 0, SEEK_END);
  int hSize = ftell(bmpFile);
  fseek(bmpFile, 0, SEEK_SET);
  // std::cout << std::endl << "Input BMP File Size (Bytes): " << hSize << std::endl;

  uint8_t* bmpCompiled = new uint8_t[hSize + 1];
  if(bmpCompiled == nullptr) exit(-1);
  fread(bmpCompiled, hSize + 1, 1, bmpFile);

  uint32_t offset = 0;

  // Extract parameters from bmpCompiled
  // Pos 0h
  h_bmpType1 = bmpCompiled[offset];
  offset++;
  h_bmpType2 = bmpCompiled[offset];
  offset++;
  // Pos 2h
  h_fileSize = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 6h
  h_reserved1 = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8);
  h_reserved2 = bmpCompiled[offset + 2] | (bmpCompiled[offset + 2] << 8);
  offset += 4;
  // Pos Ah
  h_infoOffset = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos Eh
  i_SizeHeader = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 12h
  i_Width = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 16h
  i_Height = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 1Ah
  i_Planes = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8);
  offset += 2;
  // Pos 1Ch
  i_BitCount = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8);
  offset += 2;
  // Pos 1Eh
  i_Compression = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 22h
  i_SizeImage = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 26h
  i_XPelsPerMeter = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 2Ah
  i_YPelsPerMeter = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 2Eh
  i_ClrUsed = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;
  // Pos 32h
  i_ClrImportant = bmpCompiled[offset] | (bmpCompiled[offset + 1] << 8) | (bmpCompiled[offset + 2] << 16) | (bmpCompiled[offset + 3]);
  offset += 4;

  bmpData = new uint8_t[i_SizeImage];
  for(int i = 0; i < i_SizeImage; ++i) bmpData[i] = bmpCompiled[i + offset];

  delete[] bmpCompiled;
}

uint8_t* hBMPContainer::read(){
  uint32_t widthAlign = 3 * i_Width;

  uint32_t paddingFills = 0;
  if((widthAlign) % 4 != 0) paddingFills = 4 - ((widthAlign) % 4);

  uint32_t arrHSize = (paddingFills + widthAlign);
  uint32_t arrSize = arrHSize * i_Height;

  uint8_t* bgrArr = new uint8_t[3 * i_Width * i_Height];
  uint32_t iVOffset = arrSize - arrHSize;
  uint32_t iArrOffset = 0;

  for(uint32_t row = 0; row < i_Height; row++){

      // Fill the 3 * width columns with bgr data
      for(uint32_t i = 0; i < widthAlign; i++) bgrArr[iArrOffset + i] = bmpData[iVOffset + i];
      // And the remaining columns with zero padding
      // for(uint32_t i = widthAlign; i < (widthAlign + paddingFills); i++) bmpData[iVOffset + i] = 0x00;

      // Offset Increase
      iVOffset -= arrHSize;
      iArrOffset += widthAlign;
  }

  return bgrArr;
}

uint32_t hBMPContainer::getWidth(){
  return i_Width;
}

uint32_t hBMPContainer::getHeight(){
  return i_Height;
}

void hBMPContainer::compile(){
  bmpCompiled = new uint8_t[h_fileSize];
  uint32_t offset = 0;

  // Pos 0h
  bmpCompiled[offset] = h_bmpType1;
  offset++;

  bmpCompiled[offset] = h_bmpType2;
  offset++;

  // Pos 2h
  bmpCompiled[offset] = (uint8_t)(h_fileSize & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((h_fileSize & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((h_fileSize & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((h_fileSize & 0xFF000000) >> 24);
  offset += 4;

  // Pos 6h
  bmpCompiled[offset] = (uint8_t)(h_reserved1 & 0x00FF);
  bmpCompiled[offset + 1] = (uint8_t)((h_reserved1 & 0xFF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)(h_reserved2 & 0x00FF);
  bmpCompiled[offset + 3] = (uint8_t)((h_reserved2 & 0xFF00) >> 8);
  offset += 4;

  // Pos Ah
  bmpCompiled[offset] = (uint8_t)(h_infoOffset & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((h_infoOffset & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((h_infoOffset & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((h_infoOffset & 0xFF000000) >> 24);
  offset += 4;

  // Pos Eh
  bmpCompiled[offset] = (uint8_t)(i_SizeHeader & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_SizeHeader & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_SizeHeader & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_SizeHeader & 0xFF000000) >> 24);
  offset += 4;

  // Pos 12h
  bmpCompiled[offset] = (uint8_t)(i_Width & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_Width & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_Width & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_Width & 0xFF000000) >> 24);
  offset += 4;

  // Pos 16h
  bmpCompiled[offset] = (uint8_t)(i_Height & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_Height & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_Height & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_Height & 0xFF000000) >> 24);
  offset += 4;

  // Pos 1Ah
  bmpCompiled[offset] = (uint8_t)(i_Planes & 0x00FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_Planes & 0xFF00) >> 8);
  offset += 2;

  // Pos 1Ch
  bmpCompiled[offset] = (uint8_t)(i_BitCount & 0x00FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_BitCount & 0xFF00) >> 8);
  offset += 2;

  // Pos 1Eh
  bmpCompiled[offset] = (uint8_t)(i_Compression & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_Compression & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_Compression & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_Compression & 0xFF000000) >> 24);
  offset += 4;

  // Pos 22h
  bmpCompiled[offset] = (uint8_t)(i_SizeImage & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_SizeImage & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_SizeImage & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_SizeImage & 0xFF000000) >> 24);
  offset += 4;

  // Pos 26h
  bmpCompiled[offset] = (uint8_t)(i_XPelsPerMeter & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_XPelsPerMeter & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_XPelsPerMeter & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_XPelsPerMeter & 0xFF000000) >> 24);
  offset += 4;

  // Pos 2Ah
  bmpCompiled[offset] = (uint8_t)(i_YPelsPerMeter & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_YPelsPerMeter & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_YPelsPerMeter & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_YPelsPerMeter & 0xFF000000) >> 24);
  offset += 4;

  // Pos 2Eh
  bmpCompiled[offset] = (uint8_t)(i_ClrUsed & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_ClrUsed & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_ClrUsed & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_ClrUsed & 0xFF000000) >> 24);
  offset += 4;

  // Pos 32h
  bmpCompiled[offset] = (uint8_t)(i_ClrImportant & 0x000000FF);
  bmpCompiled[offset + 1] = (uint8_t)((i_ClrImportant & 0x0000FF00) >> 8);
  bmpCompiled[offset + 2] = (uint8_t)((i_ClrImportant & 0x00FF0000) >> 16);
  bmpCompiled[offset + 3] = (uint8_t)((i_ClrImportant & 0xFF000000) >> 24);
  offset += 4;

  // Pos 36h, start of pixel array
  for(int i = offset; i < (offset + i_SizeImage); i++) bmpCompiled[i] = bmpData[i - offset];


  delete[] bmpData;
  hasCompiled = true;
}

void hBMPContainer::render(std::string filename){
  if(hasCompiled){
      // Get working directory, append file name
      std::string cwd = get_working_path();
      cwd += "/" + filename;

      // fopen compatibility
      char* cwdChar = new char[cwd.size() + 1];
      std::copy(cwd.begin(), cwd.end(), cwdChar);
      cwdChar[cwd.size()] = '\0';

      // Write file
      FILE *data = fopen(cwdChar, "w");
      fwrite(bmpCompiled, 1, h_fileSize, data);
      fclose(data);
      delete[] bmpCompiled;

      // std::cout << "Bitmap successfully written to file " << cwd << "." << std::endl;

      delete[] cwdChar;
  }
}

std::string hBMPContainer::renderAndReturnPath(std::string filename){
  if(hasCompiled){
      // Get working directory, append file name
      std::string cwd = get_working_path();
      cwd += "/" + filename;

      // fopen compatibility
      char* cwdChar = new char[cwd.size() + 1];
      std::copy(cwd.begin(), cwd.end(), cwdChar);
      cwdChar[cwd.size()] = '\0';

      // Write file
      FILE *data = fopen(cwdChar, "w");
      fwrite(bmpCompiled, 1, h_fileSize, data);
      fclose(data);
      delete[] bmpCompiled;
      // std::cout << "Bitmap successfully written to file " << cwd << "." << std::endl;

      delete[] cwdChar;
      return cwd;
  }
  return "Oops, something went wrong.";
}

hBMPContainer::~hBMPContainer() {
  // delete[] bmpCompiled;
  // delete[] bmpData;
}
