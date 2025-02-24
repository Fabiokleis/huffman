CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

all: huff
.PHONY: all

# Compila a biblioteca do professor
professor_lib.o: professor_lib.c professor_lib.h pdi.h
	$(CC) $(CFLAGS) -c professor_lib.c

pdi.o: pdi.c pdi.h
	$(CC) $(CFLAGS) -c pdi.c

arvore.o: arvore.c arvore.h
	$(CC) $(CFLAGS) -c arvore.c
Arvore.o: Arvore.c Arvore.h
	$(CC) $(CFLAGS) -c Arvore.c

stack.o: arvore.o stack.c pilha.h
	$(CC) $(CFLAGS) -c stack.c

kmeans.o: kmeans.c kmeans.h
	$(CC) $(CFLAGS) -c kmeans.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

huffman.o: stack.o heap.o hashmap.o huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

leitor_bit.o: leitor_bit.h leitor_bit.c
	$(CC) $(CFLAGS) -c leitor_bit.c

pilha.o: pilha.h pilha.c
	$(CC) $(CFLAGS) -c pilha.c

descompactador.o: Arvore.o pilha.o descompactador.h descompactador.c
	$(CC) $(CFLAGS) -c descompactador.c

main.o: leitor_bit.o descompactador.o heap.o huffman.o kmeans.o main.c
	$(CC) $(CFLAGS) -c main.c

huff: leitor_bit.o descompactador.o main.o professor_lib.o pdi.o pilha.o arvore.o heap.o hashmap.o huffman.o kmeans.o stack.o
	$(CC) $(CFLAGS) -o huff leitor_bit.o Arvore.o descompactador.o pilha.o arvore.o heap.o hashmap.o huffman.o kmeans.o stack.o main.o professor_lib.o pdi.o $(LDFLAGS)

clean:
	rm -f huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o professor_lib.o pdi.o leitor_bit.o pilha.o descompactador.o *.h4k
.PHONY: clean