/* ************************************************************************* */
/*   LTD - Tecnologia Digital 2 - Trabajo Practico 2                         */
/* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include "lib/libbmp.h"
#include "lib/utils.h"
#include "common.h"

int main(int argc, char **argv) {

  char* inputFileName;
  char* outputFileName;
  BMP* img;

  // Cargado de imagen en memoria
  loadImage(argc, argv, &inputFileName, &outputFileName, &img);
  uint32_t width = bmp_width(img);
  uint32_t height = bmp_height(img);
  uint8_t* data = bmp_data(img);

  // Solicito memoria para datos temporales
  uint8_t* brightness = (uint8_t*) malloc(width * height);
  uint8_t* edges = (uint8_t*) malloc(width * height);
  uint8_t* result = (uint8_t*) malloc(width * height * sizeof(bgra_t));

  // Procesamiento de la imagen
  step1_brightness(width, height, data, brightness, 0, width, 0, height);
  step2_edges(width, height, brightness, edges, 1, width-1, 1, height-1);
  step3_merge(width, height, data, edges, result, 1, width-1, 1, height-1);
  paintEdges(width, height, result);

  // Liberacion de memoria
  free(brightness);
  free(edges);
  free(data);

  // Guardado de la imagen
  setResultAndSaveImage(img, result, outputFileName);
  
  return 0;
}
