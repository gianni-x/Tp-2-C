/* ************************************************************************* */
/*   LTD - Tecnologia Digital 2 - Trabajo Practico 2                         */
/* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/libbmp.h"
#include "lib/utils.h"
#include "common.h"

uint32_t width;
uint32_t height;
uint8_t* data;

uint8_t* brightness;
uint8_t* edges;
uint8_t* result;

static void* process2(__attribute__((unused)) void* _) {
  step1_brightness(width, height, data, brightness, 0, width, 0, height/2);
  step2_edges(width, height, brightness, edges, 1, width-1, 1, height/2);
  step3_merge(width, height, data, edges, result, 1, width-1, 1, height/2);
}

int main(int argc, char **argv) {

  char* inputFileName;
  char* outputFileName;
  BMP* img;

  // Cargado de imagen en memoria
  loadImage(argc, argv, &inputFileName, &outputFileName, &img);
  width = bmp_width(img);
  height = bmp_height(img);
  data = bmp_data(img);

  // Solicito memoria para datos temporales
  brightness = (uint8_t*) malloc(width * height);
  edges = (uint8_t*) malloc(width * height);
  result = (uint8_t*) malloc(width * height * sizeof(bgra_t));

  // Procesamiento de la imagen
  pthread_t thread;
  pthread_create(&thread, NULL, process2, NULL);

  step1_brightness(width, height, data, brightness, 0, width, height/2-1, height);
  step2_edges(width, height, brightness, edges, 1, width-1, height/2, height-1);
  
  step3_merge(width, height, data, edges, result, 1, width-1, height/2, height-1);

  pthread_join(thread, NULL);
  paintEdges(width, height, result);

  // Liberacion de memorias
  free(brightness);
  free(edges);
  free(data);

  // Guardado de la imagen
  setResultAndSaveImage(img, result, outputFileName);

  return 0;
  }
