CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

all: huff
.PHONY: all

# Compila a biblioteca do professor
professor_lib.o: professor_lib.c
	$(CC) $(CFLAGS) -c professor_lib.c

pdi.o: pdi.c
	$(CC) $(CFLAGS) -c pdi.c

arvore.o: arvore.c
	$(CC) $(CFLAGS) -c arvore.c
Arvore.o: Arvore.c
	$(CC) $(CFLAGS) -c Arvore.c

stack.o: arvore.o stack.c
	$(CC) $(CFLAGS) -c stack.c

kmeans.o: kmeans.c
	$(CC) $(CFLAGS) -c kmeans.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c

huffman.o: stack.o heap.o hashmap.o huffman.c
	$(CC) $(CFLAGS) -c huffman.c

main.o: heap.o huffman.o kmeans.o professor_lib.o pdi.o main.c
	$(CC) $(CFLAGS) -c main.c

leitor_bit.o: leitor_bit.h leitor_bit.c
	$(CC) $(CFLAGS) -c leitor_bit.c

pilha.o: pilha.h pilha.c
	$(CC) $(CFLAGS) -c pilha.c

descompactador.o: Arvore.o pilha.o descompactador.h descompactador.c
	$(CC) $(CFLAGS) -c descompactador.c

huff: main.o
	$(CC) $(CFLAGS) -o huff professor_lib.o pdi.o arvore.o heap.o hashmap.o huffman.o kmeans.o stack.o main.o $(LDFLAGS)

descompactador: leitor_bit.o descompactador.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o descompactador leitor_bit.o Arvore.o descompactador.o pilha.o professor_lib.o pdi.o

clean:
	rm -f huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o professor_lib.o pdi.o leitor_bit.o pilha.o descompactador.o *.h4k
.PHONY: clean
