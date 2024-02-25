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

// Creamos 3 procesos, ya que necesitamos 4 procesos y el padre ya existe. Dentro de cada proceso llamamos a brightness, edges y merge.
static void* process_a(__attribute__((unused)) void* _) { 
  step1_brightness(width, height, data, brightness, 0, width/4+1, 0, height);
  step2_edges(width, height, brightness, edges, 1, width/4+1, 1, height-1);
  step3_merge(width, height, data, edges, result, 1, width/4+1, 1, height/-1);
}
static void* process_b(__attribute__((unused)) void* _) {
  step1_brightness(width, height, data, brightness, width/4-1, width/2+1, 0, height); 
  step2_edges(width, height, brightness, edges, 1, width/2+1, 1, height-1);
  step3_merge(width, height, data, edges, result, 1, width/2+1, 1, height-1);
}
static void* process_c(__attribute__((unused)) void* _) {
  step1_brightness(width, height, data, brightness, width/2-1, ((width/4)*3)+1,0,height); 
  step2_edges(width, height, brightness, edges, 1, ((width/4)*3)+1, 1, height-1);
  step3_merge(width, height, data, edges, result, 1, ((width/4)*3)+1, 1, height-1);
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
  
  // Creamos threads de los procesos que habíamos creado
  pthread_t thread_a;
  pthread_create(&thread_a, NULL, process_a, NULL); // Creamos el thread A asociado al proceso A
  
  pthread_t thread_b;
  pthread_create(&thread_b, NULL, process_b, NULL); // Creamos el thread B asociado al proceso B
  
  pthread_t thread_c;
  pthread_create(&thread_c, NULL, process_c, NULL); // Creamos el thread C asociado al proceso C
  
  step1_brightness(width, height, data, brightness,((width/4)*3)-1, width, 0, height);
  // Calcula el brillo de un arreglo de pixeles
  step2_edges(width, height, brightness, edges, ((width/4)*3)-1, width-1, 1, height-1);
  // Detecta los bordes calculando la diferencia de cambio de brillo desde un pixel al siguiente
  step3_merge(width, height, data, edges, result, ((width/4)*3)-1, width-1, 1, height-1);
  // Procesa la mezcla entre entre los pixeles de la imagen y los bordes que acabamos de detectar con step2_edges
  
  // Para obtener brightness, edges y merge alteramos el ancho (width) ya que en este archivo estamos usando threads verticales. 
  
  // Agregamos nuestros threads a la cola
  pthread_join(thread_a, NULL);
  pthread_join(thread_b, NULL);
  pthread_join(thread_c, NULL);
  
  // Liberacion de memoria
  free(brightness);
  free(edges);
  free(data);

  // Guardado de la imagen
  setResultAndSaveImage(img, result, outputFileName);
  
  return 0;
  }
