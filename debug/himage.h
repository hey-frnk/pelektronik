//
//  himage.h
//  himage
//
//  Copyright © 2017 Frank Zheng. All rights reserved.
//
#include <string>

#define RED   2
#define GREEN 1
#define BLUE  0

class hImage{
private:
  uint32_t width;
  uint32_t height;
  uint8_t* imageData;
  std::string fileName;
public:
  hImage(uint32_t, uint32_t, std::string);
  hImage(uint32_t, uint32_t);
  hImage(std::string);
  ~hImage();
  uint32_t getWidth();
  uint32_t getHeight();
  uint8_t getPixel(uint32_t, uint32_t, uint8_t);
  void toBitStream();
  void to565();
  void setPixel(uint32_t, uint32_t, uint8_t, uint8_t);
  void setPixelRGB(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
  void setBWPixel(uint32_t, uint32_t, uint8_t);
  void setRectangle(uint32_t, uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
  void setFileName(std::string);
  void save();
  void save(std::string);
  std::string saveAndReturnPath();
  std::string saveAndReturnPath(std::string);
};


class hBMPContainer{
private:
  // Bitmap file header
  uint8_t h_bmpType1;
  uint8_t h_bmpType2;
  uint32_t h_fileSize;
  uint16_t h_reserved1;
  uint16_t h_reserved2;
  uint32_t h_infoOffset;

  // Bitmap info header
  uint32_t i_SizeHeader;
  uint32_t i_Width;
  uint32_t i_Height;
  uint16_t i_Planes;
  uint16_t i_BitCount;
  uint32_t i_Compression;
  uint32_t i_SizeImage;
  uint32_t i_XPelsPerMeter;
  uint32_t i_YPelsPerMeter;
  uint32_t i_ClrUsed;
  uint32_t i_ClrImportant;

  // Bitmap file
  uint8_t* bmpCompiled;
  uint8_t* bmpData;

  bool hasCompiled;

public:
  hBMPContainer(uint8_t*, uint32_t, uint32_t);    // Write initializer
  hBMPContainer(std::string);                     // Read initializer
  uint8_t* read();                                // Read compiled data into array
  uint32_t getWidth();
  uint32_t getHeight();
  void compile();                                 // Set picture data into compilable BMP
  void render(std::string);                       // Render to file
  std::string renderAndReturnPath(std::string);   // Render to file and return path
};
