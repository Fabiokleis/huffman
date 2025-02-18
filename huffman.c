#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>

/*
  problema escolher tamanho do hash bom o suficiente...
  por padrao usa MAX_SYMBOLS pra qualquer hash
*/

#define MAX_SYMBOLS 256
#define EMPTY_CHAR '\0'
#define FILE_CHUNK_SIZE 256
#define MAX_BITS 8

uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}

Pixel unpack_color(uint32_t color_pack) {

  Pixel pixel = {0};

  pixel.r = (color_pack >> 16) & 0xFF;
  pixel.g = (color_pack >> 8);
  pixel.b = color_pack;

  return pixel;
}

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

void freq_sum(Heap* heap) {
  while (heap->size > 1) {
    Arvore* arv1 = pop(heap);
    if (0 == arv1->freq) continue; /* ignore empty */
    Arvore* arv2 = pop(heap);
    Arvore* arv = constroi_arv((uint32_t)EMPTY_CHAR, arv1->freq + arv2->freq, arv1, arv2);
    push(heap, arv);
  }
}

uint32_t count_lut_bits(uint32_t size, Hash* lut, Hash* freqs) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(lut, size, freqs[i].key);
    if (EMPTY == idx) continue;
    if (lut[idx].data.bits != NULL) {
      count += strlen(lut[idx].data.bits) * freqs[i].data.freq;
    }
  }
  return count;
}

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
    Pixel pixel = unpack_color((uint32_t)lut[idx].key);
    printf("[%8s]: color (%3d, %3d, %3d) uint32_t (%b)\n", lut[idx].data.bits, pixel.r, pixel.g, pixel.b, (uint32_t)lut[idx].key);
    return;
  }
  
  bits[altura] = '0';
  cria_huffman_code(size, arv->esq, bits, altura + 1, lut);

  bits[altura] = '1';
  cria_huffman_code(size, arv->dir, bits, altura + 1, lut);
}

void cria_huffman_lut(uint32_t size, Hash* lut, Arvore *raiz) {
  printf("-----------------------------------lut------------------------------------\n");

   /* in case of one single entry in lut */
  if (NULL == raiz->esq && NULL == raiz->dir) {
    uint32_t idx = hash_insert(lut, size, raiz->color);
    lut[idx].type = BITS;
    lut[idx].data.bits = (char*) malloc(sizeof(char)*2);
    lut[idx].data.bits[0] = '0';
    lut[idx].data.bits[1] = '\0';
    return;
  }
  char bits[MAX_BITS];
  cria_huffman_code(size, raiz, bits, 0, lut);
}

void encoda_huffman(Huffman* huff) {
  printf("---------------------------huffman-encoded--------------------------------\n");
  uint32_t j = 0;
  for (uint32_t y = 0; y < huff->height; ++y) {
    for (uint32_t x = 0; x < huff->width; ++x) {
      uint32_t color = pack_color((*huff->img)[y][x][0], (*huff->img)[y][x][1], (*huff->img)[y][x][2]);
      Pixel pixel = unpack_color(color);
      int idx = hash_search(huff->lut, MAX_SYMBOLS, color);
      if (EMPTY == idx) {
	printf("ERRO: color not found %b\n", color);
	exit(1);
      }
	  
      printf("[%8s]: color (%3d, %3d, %3d) uint32_t (%b)\n", huff->lut[idx].data.bits, pixel.r, pixel.g, pixel.b, (uint32_t)huff->lut[idx].key);
      memcpy(huff->code+j, huff->lut[idx].data.bits, strlen(huff->lut[idx].data.bits));
      j += strlen(huff->lut[idx].data.bits); 
    }
  }
  huff->code[huff->bits_count] = '\0';
}

void write_huff_bytes(FILE* f, Huffman* huff) {
  uint32_t size = floor((huff->bits_count-1) / 8) + ((huff->bits_count-1) % 8 == 0 ? 0 : 1);
  if (size == 0) size = 1; /* para char* com size menor que 8 */
  printf("input size %d bytes, output size %d bytes \n", huff->height * huff->width * 3, size);
  printf("%d/%d compressed %.2f%%\n", size, huff->height * huff->width * 3, 100.f - (((float)size / (huff->height * huff->width * 3)) * 100));
  uint8_t* bytes = (uint8_t*) malloc(sizeof(uint8_t)*size);
  if (NULL == bytes) {
    fprintf(stderr, "ERROR: failed to alloc bits %s\n", strerror(errno));
  }
  uint8_t byte = 0x0;
  uint8_t aux = 1;
  uint32_t idx = 0;
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
  bytes[idx] = byte;
  huff->h4k_size = size;
  fwrite(bytes, sizeof(uint8_t), size, f);
}

bool cmp_bits(char* r_bits, uint32_t r_size, char* bits, uint32_t b_size) {
  uint32_t min_size = (r_size < b_size) ? r_size : b_size;
  for (uint32_t i = 0; i < min_size; ++i) {
    if (r_bits[i] != bits[i]) {
      return false;
    }
  }
  return true;
}

void read_huff_bytes(uint32_t if_size, FILE* i_file, FILE* o_file, Huffman* huff) {
  printf("---------------------------huffman-decoded--------------------------------\n");
  (void) o_file;
  (void) huff;
  uint32_t s = if_size;
  if (s < FILE_CHUNK_SIZE) s = 1;
  else s = ceil(if_size / FILE_CHUNK_SIZE) + (if_size % FILE_CHUNK_SIZE == 0 ? 0 : 1);

  uint8_t* chunk = (uint8_t*)malloc(sizeof(uint8_t)* FILE_CHUNK_SIZE);
  if (NULL == chunk) {
    fprintf(stderr, "ERROR: failed to alloc chunk %s\n", strerror(errno));
  }

  uint8_t* bits = (uint8_t*)malloc(sizeof(uint8_t) * s * FILE_CHUNK_SIZE);
  if (NULL == bits) {
    fprintf(stderr, "ERROR: failed to alloc bits %s\n", strerror(errno));
  }
  uint32_t idx = 0;
  uint32_t bytes_read = 0;
  for (uint32_t i = 0; i < s; ++i) {
    bytes_read += fread(chunk, sizeof(uint8_t), FILE_CHUNK_SIZE, i_file);
    printf("bytes_read %d\n", bytes_read);
    for (uint32_t j = 0; j < bytes_read; ++j) {
      uint8_t and = 0xff;
      for (uint8_t aux = 1; aux <= 8; ++aux) {
	uint8_t bit = (chunk[j] & and) >> (8 - aux);
	//printf("chunk %b bit %b aux %d\n", chunk[j], bit, aux);
	bits[idx] = bit == 0x1 ? '1' : '0';
	idx++;
	and >>= 1;


	/* TODO:
	   traduzir a cada iteracao o bits no huff->lut
	   e escrever em o_file
	*/
	
	
      }
    }
    //memset(chunk, 0, FILE_CHUNK_SIZE);
  }

  bits[bytes_read*8] = '\0';
  printf("huffman decoded %s\n", bits);
}

Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->img = img;
  huff->height = height;
  huff->width = width;
  Hash* freqs = color_freqs(huff);
  //hash_print(freqs, MAX_SYMBOLS);

  huff->heap = criar_heap(MAX_SYMBOLS);
  for (uint32_t i = 0; i < MAX_SYMBOLS; ++i) {
    if (EMPTY != freqs[i].key) {
      push(huff->heap, constroi_arv((uint32_t)freqs[i].key, freqs[i].data.freq, NULL, NULL));
    }
  }

  freq_sum(huff->heap);
  huff->lut = create_hash(MAX_SYMBOLS);
  huff->root = pop(huff->heap);
  //printf("root freq: %d\n", root->freq);
  cria_huffman_lut(MAX_SYMBOLS, huff->lut, huff->root);
  //hash_print(huff->lut, MAX_SYMBOLS);
  huff->bits_count = count_lut_bits(MAX_SYMBOLS, huff->lut, freqs) + 1;
  printf("huffman encoded bits len %d \n", huff->bits_count);

  huff->code = (char*)malloc(huff->bits_count * sizeof(char));
  encoda_huffman(huff);

  return huff;
}

/*
Huffman* constroi_huff(char* bytes) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->bytes = bytes;
  huff->bytes_count = strlen(bytes);
  huff->lut_size = 0;
  Hash *freqs = bytes_freqs(huff->bytes, &huff->lut_size);
  printf("lut size: %d\n", huff->lut_size);
  //hash_print(freqs, (uint32_t)strlen(bytes));

  
  freq_sum(huff->heap);
  //printf("heap size: %d\n", huff->heap->size);

  huff->lut = create_hash(huff->lut_size);
  huff->root = pop(huff->heap);
  //printf("root freq: %d\n", root->freq);
  cria_huffman_lut(huff->lut_size, huff->lut, huff->root);
  huff->bits_count = count_lut_bits(MAX_SYMBOLS, huff->lut, freqs) + 1;

  //printf("bit count: %d\n", bit_count);
  huff->code = (char*)malloc(sizeof(char)*huff->bits_count);
  encoda_huffman(huff->lut_size, huff->lut, huff->code, huff->bytes);
  huff->code[huff->bits_count] = '\0';
  
  return huff;
}
*/
