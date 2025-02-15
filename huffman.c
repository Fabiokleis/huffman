#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX_SYMBOLS 127
#define EMPTY_CHAR '\0'
#define FILE_CHUNK_SIZE 256

Hash* bytes_freqs(char *bytes, uint32_t *count) {
  uint32_t size = (uint32_t)strlen(bytes);
  Hash* hash = create_hash(MAX_SYMBOLS);
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(hash, MAX_SYMBOLS, (int)bytes[i]);\
    if (EMPTY == idx) {
      uint32_t n_idx = hash_insert(hash, MAX_SYMBOLS, (int)bytes[i]);
      hash[n_idx].type = FREQ;
      hash[n_idx].data.freq = 1;
      *count += 1;
    } else {
      hash[idx].type = FREQ;
      hash[idx].data.freq = hash[idx].data.freq + 1;      
    }
  }
  return hash;
}

void freq_sum(Heap* heap) {
  while (heap->size > 1) {
    Arvore* arv1 = pop(heap);
    if (0 == arv1->freq) continue; /* ignore empty */
    Arvore* arv2 = pop(heap);
    Arvore* arv = constroi_arv(EMPTY_CHAR, arv1->freq + arv2->freq, NULL, NULL);
    arv->esq = arv1;
    arv->dir = arv2;
    push(heap, arv);
  }
}

uint32_t count_lut_bits(uint32_t size, Hash* lut, Hash* freqs) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(lut, size, (int)freqs[i].key);
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
    uint32_t idx = hash_insert(lut, size, arv->byte);
    lut[idx].type = BITS;
    lut[idx].data.bits = strdup(bits);
    printf("[%s] bits %c\n", lut[idx].data.bits, (char)lut[idx].key);
    return;
  }

  bits[altura] = '0';
  cria_huffman_code(size, arv->esq, bits, altura + 1, lut);

  bits[altura] = '1';
  cria_huffman_code(size, arv->dir, bits, altura + 1, lut);
}

void cria_huffman_lut(uint32_t size, Hash* lut, Arvore *raiz) {
  char bits[raiz->freq];
  cria_huffman_code(size, raiz, bits, 0, lut);
}

void encoda_huffman(uint32_t lut_size, Hash* lut, char* out, char* bytes) {
  uint32_t size = strlen(bytes);
  uint32_t j = 0;
  
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(lut, lut_size, bytes[i]);
    if (EMPTY == idx) return;

    //printf("%c: %s\n", (char)lut[idx].key, lut[idx].data.bits);
    memcpy(out+j, lut[idx].data.bits, strlen(lut[idx].data.bits));
    j += strlen(lut[idx].data.bits);
  }
}

void write_huff_bytes(FILE* f, Huffman* huff) {
  uint32_t size = floor((huff->bits_count-1) / 8) + ((huff->bits_count-1) % 8 == 0 ? 0 : 1);
  if (size == 0) size = 1; /* para char* com size menor que 8 */
  printf("input size %d, output size %d \n", huff->bytes_count, size);
  printf("%d/%d compressed %.2f%%\n", size, huff->bytes_count, 100.f - (((float)size / (huff->bytes_count)) * 100));
  uint8_t* bytes = (uint8_t*) malloc(sizeof(uint8_t)*size);
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

void read_huff_bytes(uint32_t if_size, FILE* i_file, FILE* o_file, Huffman* huff) {
  (void) o_file;
  (void) huff;
  uint32_t s = if_size;
  if (s < FILE_CHUNK_SIZE) s = 1;
  else s = ceil(if_size / FILE_CHUNK_SIZE) + (if_size % FILE_CHUNK_SIZE == 0 ? 0 : 1);

  uint8_t* chunk = (uint8_t*)malloc(sizeof(uint8_t)* FILE_CHUNK_SIZE);
  uint8_t* bits = (uint8_t*)malloc(sizeof(uint8_t) * s * FILE_CHUNK_SIZE);
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
  printf("codigo lido: %s\n", bits);
}

Huffman* constroi_huff(char* bytes) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->bytes = bytes;
  huff->bytes_count = strlen(bytes);
  huff->lut_size = 0;
  Hash *freqs = bytes_freqs(huff->bytes, &huff->lut_size);
  printf("lut size: %d\n", huff->lut_size);
  //hash_print(freqs, (uint32_t)strlen(bytes));

  huff->heap = criar_heap(MAX_SYMBOLS);

  for (uint32_t i = 0; i < MAX_SYMBOLS; ++i) {
    push(huff->heap, constroi_arv((char)freqs[i].key, freqs[i].data.freq, NULL, NULL));
  }
  
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
