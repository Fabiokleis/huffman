#include <stdio.h>
#include "fila.h"
#include "huffman.h"

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;

  char* texto = "hello, world!";
  
  Huffman* huff = constroi_huff(texto);

  printf("texto: %s\n", huff->bytes);
  
  return 0;
}
