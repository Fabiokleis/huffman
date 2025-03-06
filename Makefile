CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

all: huff descompactador
.PHONY: all

# Compila a biblioteca do professor
pdi.o: pdi.c pdi.h
	$(CC) $(CFLAGS) -c pdi.c

arvore.o: arvore.c arvore.h
	$(CC) $(CFLAGS) -c arvore.c

stack.o: arvore.o stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

kmeans.o: kmeans.c kmeans.h
	$(CC) $(CFLAGS) -c kmeans.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

huffman.o: stack.o heap.o hashmap.o huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

huff.o: heap.o huffman.o kmeans.o pdi.o huff.c
	$(CC) $(CFLAGS) -c huff.c

huff: huff.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o huff pdi.o arvore.o heap.o hashmap.o huffman.o kmeans.o stack.o huff.o

Arvore.o: Arvore.c Arvore.h
	$(CC) $(CFLAGS) -c Arvore.c

pilha.o: pilha.c pilha.h
	$(CC) $(CFLAGS) -c pilha.c

leitor_bit.o: leitor_bit.c leitor_bit.h
	$(CC) $(CFLAGS) -c leitor_bit.c

imagem.o: imagem.c imagem.h
	$(CC) $(CFLAGS) -c imagem.c

descompactador.o: leitor_bit.o pilha.o Arvore.o imagem.o descompactador.c descompactador.h
	$(CC) $(CFLAGS) -c descompactador.c

descompactador:	descompactador.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o descompactador Arvore.o pilha.o leitor_bit.o imagem.o descompactador.o

clean:
	rm -f huff descompactador *.o *.h4k
