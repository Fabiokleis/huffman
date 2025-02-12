#include <stdio.h>
#include "heap.h"
#include "huffman.h"

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  char* texto = "hello";
  printf("texto: %s\n", texto);
  
  Huffman* huff = constroi_huff(texto);

  printf("texto: %s\n", huff->bytes);
  printf("codigo: %s\n", huff->code);
  
  return 0;
}
