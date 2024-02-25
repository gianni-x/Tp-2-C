#include <stdio.h>

int len(char* s) {
  int contador = 0;
  while(*s != NULL){
    contador ++;
    s++;
  }
  return contador;
}

char* copy(char* s) {
  char* copia = (char*)malloc((len(s) + 1) * sizeof(char));
  for (int i = 0; i < len(s); i++) {
        copia[i] = s[i];
    }
    copia[len(s)] = '\0'; // Agrega el carácter nulo al final
    return copia;
}

void replaceChar(char* s, char old, char new){
  for (int i=0; i < len(s); i++){
    if (s[i] == old){
      s[i] = new;
    }
  } 
}

char* concatenate(char* s1, char* s2){
  char* unido = (char*)malloc((len(s1) + len(s2) + 1) * sizeof(char));
  for (int i = 0; i < len(s1); i++) {
        unido[i] = s1[i];
    }
  for (int i = 0; i < len(s2); i++) {
        unido[len(s1) + i + 1] = s2[i];
    }
  unido[len(s1) + len(s2)] = '\0'; // Agrega el carácter nulo al final
    return unido;
}

int main(){
  char* texto = "Renacuajo";
  int longitud = len(texto);
  char* copia = copy(texto);
  char ejemplo[] = "Renacuajo";
  char* concatenado = concatenate("Argentina ", "Campeón");
  printf("La longitud del string es: %d\n", longitud);
  printf("Y su copia es: %s\n", copia);
  replaceChar(ejemplo, 'a', 'o');
  printf("Remplazo: %s\n", ejemplo);
  printf("Concatención: %s\n", concatenado);
  free(copia);
  free(concatenado);
  return 0;
}