#include <stdio.h>
#include "heap.h"
#include "huffman.h"

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  char* texto = "hello";
  
  Huffman* huff = constroi_huff(texto);

  printf("texto: %s\n", huff->bytes);
  
  return 0;
}
