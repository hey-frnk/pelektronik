#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdint.h>

// Bibliothek zur Erstellung von BMP-Grafiken

#include "himage.h"
#include "../font.h"
#include "../mdisplay_color.h"
#include "../mdisplay_hlvf.h"

hImage *_crObj = nullptr;

using namespace std;

// Rotate it right.
void transpose(hImage* image1){
  for(int i = 0; i < image1->getWidth(); ++i){
    for(int j = 0; j < i; ++j){
      for(int k = 0; k < 3; ++k){
        uint8_t tmp = image1->getPixel(i, j, k);
        image1->setPixel(i, j, image1->getPixel(j, i, k), k);
        image1->setPixel(j, i, tmp, k);
      }
    }
  }
}

// Mit deallocateMatrix soll eine nicht mehr benoetigte Matrix freigegeben werden
void deallocateMatrix(double** arr, int n){
  for(int i = 0; i < n; ++i) delete[] arr[i];
  delete[] arr;
}


int main(int argc, char** argv){
  // Matrix-Dimension (n x n)
  // = Anzahl der Bildpunkte in der Laenge/Breite
  int n = 160;
  char str1[] = "On some nights I still believe that a car with the gas needle on empty can run about fifty more miles with the right music on the radio";
  // char str2[] = "11:53";

  _crObj = new hImage(n, n, "test.bmp");

  mdisplay_hlvf_FillScreen(COLOR_WHITE);
  mdisplay_hlvf_DrawColorWheelString(1, 0, str1, 0, 255, 153, 77, 0, ALIGNMENT_LEFT);
  mdisplay_hlvf_FillRectangle(10, 10, 20, 15, COLOR_BLACK);
  //mdisplay_hlvf_DrawColorWheelString(1, 10, str1, 76, 150, 153, 77, 1);
  // mdisplay_hlvf_DrawColorWheelString(2, 41, str1, 151, 250, 153, 77, 2);
  // mdisplay_hlvf_DrawString(2, 90, str2, COLOR_GRAYBLUE, 3);

  for(uint16_t i = 0; i < 256; ++i) printf("0x%hx, ", mdisplay_hsl_to565(i, 153, 77));

  mdisplay_hlvf_DrawIcon(0, 60, HEART24, COLOR_RED);
  mdisplay_hlvf_DrawIcon(26, 60, HEART16, COLOR_RED);
  mdisplay_hlvf_DrawIcon(45, 60, RABBIT16, COLOR_GRAY);

  for(int i = NAV_PLAY; i <= NAV_SOUND; ++i) mdisplay_hlvf_DrawIcon(0 + 20 * (i - NAV_PLAY), 85, i, COLOR_BLACK);
  for(int i = NAV_REPA; i <= NAV_SHUFFLE; ++i) mdisplay_hlvf_DrawIcon(0 + 20 * (i - NAV_REPA), 103, i, COLOR_BLACK);

  transpose(_crObj);

  cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
  delete _crObj;


  if (argc == 1){
    cout << "No bitmap conversion requested" << endl;
    return 0;
  }

  string fileName = argv[1];

  hImage* renderT = new hImage("../bitmap/" + fileName);
  cout << "static unsigned char const 16x16[] = {" << endl;
  renderT->toBitStream();
  cout << "};" << endl;
  delete renderT;


  /*uint8_t _cptr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  for(uint8_t i = 0; i < 3; ++i){
		for(uint8_t j = 0; j < 3; ++j){
			printf("0x%hhx, ", _cptr[3 * i + j]);
			//st7735_WritePixel(x + i, y + j, mdisplay_rgb_to565(_cptr[i * j + j], _cptr[i * j + j], _cptr[i * j + j]));
		}
		printf("\n");
	}*/

}
