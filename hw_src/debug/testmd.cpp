#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdint.h>

// Bibliothek zur Erstellung von BMP-Grafiken

#include "himage.h"
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


int main(){
  // Matrix-Dimension (n x n)
  // = Anzahl der Bildpunkte in der Laenge/Breite
  int n = 160;
  char str1[] = "Downtown we let it go";
  char str2[] = "08:43";

  _crObj = new hImage(n, n, "test.bmp");

  mdisplay_hlvf_FillScreen(COLOR_WHITE);
  mdisplay_hlvf_DrawColorWheelString(1, 0, str1, 0, 75, 153, 77, 0);
  mdisplay_hlvf_DrawColorWheelString(1, 10, str1, 76, 150, 153, 77, 1);
  mdisplay_hlvf_DrawColorWheelString(2, 41, str1, 151, 250, 153, 77, 2);
  mdisplay_hlvf_DrawString(2, 90, str2, COLOR_GRAYBLUE, 3);
  //mdisplay_hlvf_DrawColorWheelString(2, 0, str2, 71, 213, 153, 77, 2);

  //mdisplay_hlvf_DrawChar(2, 0, 'A', COLOR_CYAN, 0);
  //mdisplay_hlvf_DrawChar(2, 20, 'A', COLOR_BLUE, 1);
  //mdisplay_hlvf_DrawChar(2, 40, 'A', COLOR_GREEN, 2);
  //mdisplay_hlvf_DrawChar(2, 70, 'A', COLOR_RED, 3);

  transpose(_crObj);

  cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
  delete _crObj;

}
