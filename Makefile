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

arvore.o: arvore.c
	$(CC) $(CFLAGS) -c arvore.c

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

main.o: huffman.o kmeans.o main.c
	$(CC) $(CFLAGS) -c main.c

huff: main.o professor_lib.o pdi.o
	$(CC) $(CFLAGS) -o huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o professor_lib.o pdi.o $(LDFLAGS)

clean:
	rm -f huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o professor_lib.o pdi.o *.h4k