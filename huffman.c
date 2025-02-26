#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include "stack.h"

/*
  problema escolher tamanho do hash bom o suficiente...
  por padrao usa MAX_SYMBOLS pra qualquer hash
*/

#define MAX_SYMBOLS 256
#define EMPTY_CHAR '\0'
#define MAX_BITS 256

/*
  calcula a frequencia de cada cor da img
  inserindo num Hash*
  @huff Huffman* struct huffman
*/
Hash* color_freqs(Huffman* huff) {
  Hash* hash = create_hash(MAX_SYMBOLS);
  
  for (uint32_t y = 0; y < huff->height; ++y) {
    for (uint32_t x = 0; x < huff->width; ++x) {
      uint32_t color = pack_color((*huff->img)[y][x][0], (*huff->img)[y][x][1], (*huff->img)[y][x][2]);
      int idx = hash_search(hash, MAX_SYMBOLS, color);
      if (EMPTY == idx) {
	uint32_t n_idx = hash_insert(hash, MAX_SYMBOLS, color);
	hash[n_idx].type = FREQ;
	hash[n_idx].data.freq = 1;
      } else {
	hash[idx].type = FREQ;
	hash[idx].data.freq = hash[idx].data.freq + 1;      
      }
    }
  }
  
  return hash;
}

/*
  cria a arvore de huffman fazendo pop da heap
  com todas as frequencias e faz push do node
  esquerdo e direito no heap somando frequencias
  a cada 2 pops.

  @heap Heap* heap dos nodes da arvore
*/
void freq_sum(uint32_t* total_nodes, Heap* heap) {
  while (heap->size > 1) {
    Arvore* arv1 = pop(heap);
    if (0 == arv1->freq) continue; /* ignore empty */
    Arvore* arv2 = pop(heap);
    Arvore* arv = constroi_arv((uint32_t)EMPTY_CHAR, arv1->freq + arv2->freq, arv1, arv2);
    (*total_nodes) += 3;
    push(heap, arv);
  }
}

/*
  conta o total de bits necessarios para escrever
  o huffman ja codificado, calcula lendo cada frequencia
  multiplicando pelo strlen dos bits.

  @size uint32_t tamanho do hash de frequencias
  @freqs Hash* hashmap das frequencias
  @lut Hash* lookup table ja preenchida
*/
uint32_t count_lut_bits(uint32_t size, Hash* lut, Hash* freqs) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < size; ++i) {
    if(freqs[i].key == EMPTY) continue;
    //printf("freqs key %d freq %d\n", freqs[i].key, freqs[i].data.freq);
    int idx = hash_search(lut, size, freqs[i].key);
    if (EMPTY == idx) exit(1);
    //printf("lut data bits %s freq %d\n", lut[idx].data.bits, freqs[i].data.freq);
    if (lut[idx].data.bits != NULL) {
      count += strlen(lut[idx].data.bits) * freqs[i].data.freq;
    }
  }
  return count;
}

/*
  funcao que percorre a arvore pre-ordem adicionando bits
  conforme anda para esquerda e para direita em char* bits,
  quando encontrado node com esq e dir NULL e adicionado na
  Hash* lut.

  @size uint32_t tamanho do hashmap da lut
  @arv Arvore* node que esta sendo percorrido (raiz por primeiro)
  @altura uint32_t numero de bits percorridos a cada chamada recursiva
  @lut *Hash lookup table a ser constriuida
*/
void cria_huffman_code(uint32_t size, Arvore* arv, char* bits, uint32_t altura, Hash* lut) {
  if (arv == NULL) return;
  if (arv->esq == NULL && arv->dir == NULL) {
    bits[altura] = '\0';
    int idx = hash_insert(lut, size, arv->color);
    if (EMPTY == idx) {
      printf("ERROR: not found idx %d bits %s\n", idx, bits);
      exit(1);
    }
    lut[idx].type = BITS;
    lut[idx].data.bits = strdup(bits);
    //Pixel pixel = unpack_color((uint32_t)lut[idx].key);
    //printf("[%8s]: color (%3d, %3d, %3d) uint32_t (%b)\n", lut[idx].data.bits, pixel.r, pixel.g, pixel.b, (uint32_t)lut[idx].key);
    return;
  }
  
  bits[altura] = '0';
  cria_huffman_code(size, arv->esq, bits, altura + 1, lut);

  bits[altura] = '1';
  cria_huffman_code(size, arv->dir, bits, altura + 1, lut);
}

/*
  cria a lut percorrendo a raiz da arvore de huffman
  
  @size uint32_t tamanho da lut
  @lut Hash* lookup table a ser preenchida
  @raiz Arvore* ultimo item da heap (arvore de huffman ja construida)
  
*/
void cria_huffman_lut(uint32_t size, Hash* lut, Arvore* raiz) {
  //printf("-----------------------------------lut------------------------------------\n");

   /* in case of one single entry in lut */
  if (NULL == raiz->esq && NULL == raiz->dir) {
    uint32_t idx = hash_insert(lut, size, raiz->color);
    lut[idx].type = BITS;
    lut[idx].data.bits = (char*) malloc(sizeof(char)*2);
    lut[idx].data.bits[0] = '0';
    lut[idx].data.bits[1] = '\0';
    return;
  }
  char* bits = (char*) malloc(sizeof(char) * MAX_BITS);
  cria_huffman_code(size, raiz, bits, 0, lut);
  free(bits);
}

/*
  percorre a imagem pega cada cor e olha na lut
  e escreve a codificacao dentro do huff->code

  @huff Huffman* struct huffman img e lut
*/
void encoda_huffman(Huffman* huff) {
  //printf("---------------------------huffman-encoded--------------------------------\n");
  uint32_t j = 0;
  for (uint32_t y = 0; y < huff->height; ++y) {
    for (uint32_t x = 0; x < huff->width; ++x) {
      uint32_t color = pack_color((*huff->img)[y][x][0], (*huff->img)[y][x][1], (*huff->img)[y][x][2]);
      //Pixel pixel = unpack_color(color);
      int idx = hash_search(huff->lut, MAX_SYMBOLS, color);
      if (EMPTY == idx) {
	printf("ERRO: color not found %b\n", color);
	exit(1);
      }
	  
      //printf("[%8s]: color (%3d, %3d, %3d) uint32_t (%b)\n", huff->lut[idx].data.bits, pixel.r, pixel.g, pixel.b, (uint32_t)huff->lut[idx].key);
      memcpy(huff->code+j, huff->lut[idx].data.bits, strlen(huff->lut[idx].data.bits));
      j += strlen(huff->lut[idx].data.bits); 
    }
  }
  huff->code[huff->bits_count-1] = '\0';
}

/*
  escreve em binario os bits do char* code das
  cores ja codificadas com huffman no arquivo de saida.
  
  @f FILE* arquivo de escrita binaria
  @offset uint8_t total de bits a serem pulados
  @huff Huffman* struct huffman com a lut ja feita e o char* code

  @return uint8_t offset de bits do ultimo byte
*/
uint8_t write_huff_bytes(FILE* f, uint8_t offset, Huffman* huff) {
  uint32_t size = floor((huff->bits_count-1) / 8) + ((huff->bits_count-1) % 8 == 0 ? 0 : 1);
  if (size == 0) size = 1; /* para char* com size menor que 8 */
  //printf("input size %d bytes, output size %d bytes \n", huff->height * huff->width * 3, size);
  //printf("%d/%d compressed %.2f%%\n", size, huff->height * huff->width * 3, 100.f - (((float)size / (huff->height * huff->width * 3)) * 100));

  //if (offset > 0) size += 1;
  uint8_t* bytes = (uint8_t*) malloc(size*sizeof(uint8_t)); /* vetor com bytes a serem escritos */
  if (NULL == bytes) {
    fprintf(stderr, "ERROR: failed to alloc bits %s\n", strerror(errno));
  }
  
  uint8_t byte = 0x0; /* byte que sera preenchido com cada bit */
  uint8_t aux = 1; /* aux para shiffitar para esquerda os bits de byte */
  uint32_t idx = 0;
  if (offset > 0) {
    uint8_t last_byte[1];
    uint32_t file_size = ftell(f);
    fseek(f, file_size-1, SEEK_SET);
    uint32_t b_read = fread(last_byte, sizeof(uint8_t), 1, f);
    if (b_read == 0) {
      fprintf(stderr, "ERROR: failed to read bytes %s\n", strerror(errno));
      exit(1);
    }
    aux = offset+1;
    byte = last_byte[0];
    //printf("offset byte %08b\n", byte);
  }
  
  for (uint32_t i = 0; i < huff->bits_count-1; ++i) {
    if (huff->code[i] == '1') {
      byte |= 1 << (8  - aux);
      //printf("aux %d bit %d byte %b\n", aux, (8  - aux), byte);
    }
    if (aux == 8) {
      bytes[idx] = byte;
      idx++;
      //printf("byte %b\n", byte);
      byte = 0x0;
      aux = 0;
    }
    aux++;
  }
  bytes[idx] = byte; /* ultimo byte restante */
  huff->h4k_size = size;
  if (offset > 0) {
    uint32_t file_size = ftell(f);
    fseek(f, file_size-1, SEEK_SET); // retorna ultimo byte
  }

  fwrite(bytes, sizeof(uint8_t), size, f);

  free(bytes);

  return aux > 0 ? aux-1 : 0; // offset
}

/*
  pega arvore de huffman e copia os bits em bytes

  @arv Arvore* struct arvore com as cores e altura
  @bytes uint8_t* bytes com os bits dos nos e cores
*/
uint8_t encoda_huff_tree(Arvore* raiz, uint8_t offset, uint8_t* bytes) {
  Stack* stack = create_stack();

  uint32_t cursor = 0;
  uint32_t bits_offset = offset;

  while (!stack_is_empty(stack)) {
    Arvore* arv = stack_pop(stack);
    
    if (bits_offset == 4) {
      bits_offset = 0;
      cursor++;
      //printf("cursor dentro %d\n", cursor);
    }

    // copia 11 para os dois bits mais significativos
    if (arv->esq != NULL && arv->dir != NULL) {
      bytes[cursor] |= 0x3 << (6 - (2 * bits_offset));
      bits_offset += 1;
      //printf("no 11\n");
    }
    
    // copia 10 para os dois bits mais significativos
    if (arv->esq != NULL && arv->dir == NULL) {
      bytes[cursor] |= 0x2 << (6 - (2 * bits_offset));
      bits_offset += 1;
      //printf("no 10\n");
    }

    // copia 01 para os dois bits mais significativos
    if (arv->esq == NULL && arv->dir != NULL) {
      bytes[cursor] |= 0x1 << (6 - (2 * bits_offset));
      bits_offset += 1;
      //printf("no 01\n");
    }

    /*
      copia 00 para os dois bits mais significativos
      e as cores em sequencia, ajusta os offsets de bits
      para fitar os 24 bits em sequencia.
    */
    if (arv->esq == NULL && arv->dir == NULL) {
      // printf("no 00\n");
      //bytes[cursor] |= 0x0 << (6 - (2 * bits_offset));
      //printf("offset %d\n", bits_offset);

      Pixel pixel = unpack_color(arv->color);
      //printf("cursor %d color (%08b, %08b, %08b)\n", cursor, pixel.r, pixel.g, pixel.b);

      if (bits_offset == 0) {
	/* 00 + 3 canais, shiffta 2 e 6 alternadamente */
	bytes[cursor] |= (pixel.r >> 2);
	//printf("R bytes: %d\n", bytes[cursor]);
	bytes[++cursor] |= (pixel.r << 6);
	//printf("R bytes: %d\n", bytes[cursor]);
	bytes[cursor] |= (pixel.g >> 2);
	//printf("G bytes: %d\n", bytes[cursor]);
	bytes[++cursor] |= (pixel.g << 6);
	//printf("G bytes: %d\n", bytes[cursor]);
	bytes[cursor] |= (pixel.b >> 2);
	//printf("B bytes: %d\n", bytes[cursor]);
	bytes[++cursor] |= (pixel.b << 6);
	bits_offset = 1;
	//printf("B bytes: %d\n", bytes[cursor]);
      } else if (bits_offset == 1) {
	/*
	  0x0F 15  00001111
	  0xF0 240 11110000
	*/	
	bytes[cursor] |= (pixel.r >> 4);// & 0x0F;
	bytes[++cursor] |= (pixel.r << 4);// & 0xF0;
	//printf("R bytes: %08b\n", bytes[cursor]);
	bytes[cursor] |= (pixel.g >> 4);
	//printf("G bytes: %08b\n", bytes[cursor]);
	bytes[++cursor] |= (pixel.g << 4);
	//printf("G bytes: %08b\n", bytes[cursor]);
	bytes[cursor] |= (pixel.b >> 4);
	//printf("B bytes: %08b\n", bytes[cursor]);
	bytes[++cursor] |= (pixel.b << 4);
	//printf("B bytes: %08b\n", bytes[cursor]);
	bits_offset = 2;
      } else if (bits_offset == 2) {
	/* 2 * bits_offset + 00 + 3 canais, shiffta 6 e 2 alternadamente */
	bytes[cursor] |= (pixel.r >> 6);
	//printf("R bytes: %08b %08b %08b\n", pixel.r, pixel.r >> 6, bytes[cursor]);
	bytes[++cursor] |= (pixel.r << 2);
	//printf("R bytes: %08b %08b %08b\n", pixel.r, pixel.r << 2, bytes[cursor]);
	bytes[cursor] |= (pixel.g >> 6);
	//printf("G bytes: %08b %08b %08b\n", pixel.g, pixel.g >> 6, bytes[cursor]);
	bytes[++cursor] |= (pixel.g << 2);
	//printf("G bytes: %08b %08b %08b\n", pixel.g, pixel.g << 2, bytes[cursor]);
	bytes[cursor] |= (pixel.b >> 6);
	//printf("B bytes: %08b %08b %08b\n", pixel.b, pixel.b >> 6, bytes[cursor]);
	bytes[++cursor] |= (pixel.b << 2); 
	//printf("B bytes: %08b %08b %08b\n", pixel.b, pixel.b << 2, bytes[cursor]);
	bits_offset = 3;
      } else if (bits_offset == 3) {
	/* 2 * bits_offset + 00 = 8 bits, faz fit em cada byte */
	bytes[++cursor] = pixel.r;
	bytes[++cursor] = pixel.g;
	bytes[++cursor] = pixel.b;
	cursor++; // next byte
	bits_offset = 0;
      }
      //printf("cursor %d\n", cursor);
    }

    if (arv->esq != NULL) stack_push(stack, arv->esq);
    if (arv->dir != NULL) stack_push(stack, arv->dir);
  }

  //printf("stack %d %d\n", cursor, total_bits * 2);
  /* for (uint32_t i = 0; i < stack->; ++i) { */
  /*   if (heap->vetor[i] != NULL) printf("%08b\n", heap->vetor[i]->color); */
  /* } */
  while(stack_pop(stack) != NULL) {
    // printf("stack %d %d\n", cursor, total_bits * 2);
  }

  return bits_offset;
}

/*
  codifica a arvore de huffman e escreve em um arquivo binario

  @raiz Arvore* arvore huffman com as cores
  @offset uint8_t bits restantes do final da ultima escrita
  @file FILE* arquivo para escrita em binario
*/ 
uint8_t write_huff_tree(Huffman* huff, uint8_t offset, FILE* file) {
  uint32_t nodes_bytes = (uint32_t) ceil(huff->total_nodes * 2.f / 8.f);
  //printf("nodes bytes %d\n", nodes_bytes);
  uint32_t total_bytes = nodes_bytes + (huff->total_colors * 3);
  //printf("total bytes %d\n", total_bytes);
  huff->tree_size = total_bytes;
  uint8_t* bytes = (uint8_t*)malloc(total_bytes * sizeof(uint8_t));
  //pre_imprime_arv(huff->root);

  if (offset > 0) {
      uint8_t last_byte[1];
      uint32_t file_size = ftell(file);
      fseek(file, file_size-1, SEEK_SET);
      uint32_t b_read = fread(last_byte, sizeof(uint8_t), 1, file);
      if (b_read == 0) {
	fprintf(stderr, "ERROR: failed to read bytes %s\n", strerror(errno));
	exit(1);
      }
      bytes[0] = last_byte[0];
      fseek(file, file_size-1, SEEK_SET); // retorna ultimo byte
      total_bytes++;
  }
  uint8_t bits_offset = encoda_huff_tree(huff->root, offset, bytes);

  /* printf("bytes\n"); */
  /* for (uint32_t i = 0; i < total_bytes; ++i) { */
  /*   printf("%08b\n", bytes[i]); */
  /* } */
  fwrite(bytes, sizeof(uint8_t), total_bytes, file);
  free(bytes);
  return bits_offset;
}

/*
  faz a codificacao de huffman, conta frequencia e monta
  a arvore de huffman usando um heap minimo e constroi
  a lookup table que codifica as cores
  
  @img uint8_t**** ponteiro para imagem normalizada
  @height uint32_t height altura da imagem
  @width uint32_t width largura da imagem
*/
Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->img = img;
  huff->height = height;
  huff->width = width;
  Hash* freqs = color_freqs(huff);
  //hash_print(freqs, MAX_SYMBOLS);

  /* cria heap minimo com todas nodes com os valores das frequencias */
  huff->heap = criar_heap(MAX_SYMBOLS);
  huff->total_colors = 0;
  for (uint32_t i = 0; i < MAX_SYMBOLS; ++i) {
    if (EMPTY != freqs[i].key) {
      push(huff->heap, constroi_arv((uint32_t)freqs[i].key, freqs[i].data.freq, NULL, NULL));
      huff->total_colors++;
    }
  }

  //printf("total colors: %d\n", huff->total_colors);

  freq_sum(&huff->total_nodes, huff->heap); /* monta arvore */
  huff->lut = create_hash(MAX_SYMBOLS);
  huff->root = pop(huff->heap); /* ultimo item do heap minimo e a raiz da arvore */
  //huff->total_nodes = conta_nodes(huff->root, 0);
  
  //printf("total nodes: %d\n", huff->total_nodes);
  cria_huffman_lut(MAX_SYMBOLS, huff->lut, huff->root);
  //hash_print(huff->lut, MAX_SYMBOLS);
  huff->bits_count = count_lut_bits(MAX_SYMBOLS, huff->lut, freqs) + 1;
  //printf("huffman encoded bits len %d \n", huff->bits_count-1);

  huff->code = (char*)malloc(huff->bits_count * sizeof(char));
  encoda_huffman(huff);
  if (freqs != NULL)
    free(freqs);
  
  if (huff->lut != NULL) {
    for (uint32_t i = 0; i < MAX_SYMBOLS; ++i) {
      if (EMPTY != huff->lut[i].key) {
	free(huff->lut[i].data.bits);
      }
    }
    free(huff->lut);
  }

  return huff;
}
