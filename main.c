#include <stdio.h>
#include "huffman.h"

#define CODE_FILE "code.h4k"

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  //char* texto = "Então, foram chamados os escrivães do rei, naquele mesmo tempo e no mês terceiro (que é o mês de sivã), aos vinte e três do mesmo, e se escreveu conforme tudo quanto ordenou Mardoqueu aos judeus, como também aos sátrapas, e aos governadores, e aos maiorais das províncias que se estendem da Índia até à Etiópia, cento e vinte e sete províncias, a cada província segundo a sua escritura e a cada povo conforme a sua língua; como também aos judeus segundo a sua escritura e conforme a sua língua";
  char* texto = "hello";
  printf("texto: %s\n", texto);
  
  Huffman* huff = constroi_huff(texto);

  printf("codigo: %s\n", huff->code);

  FILE* f = NULL;
  f = fopen(CODE_FILE, "wb");
  if (f == NULL) {
    printf("falha abrir arquivo para escrita.\n");
    return 1;
  }
  
  write_huff_bytes(f, huff);
  printf("gerado arquivo: %s\n", CODE_FILE);
  
  return 0;
}
