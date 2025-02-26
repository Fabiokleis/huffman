CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

all: huff
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

huff.o: heap.o huffman.o kmeans.o professor_lib.o pdi.o huff.c
	$(CC) $(CFLAGS) -c huff.c

huff: huff.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o huff pdi.o arvore.o heap.o hashmap.o huffman.o kmeans.o stack.o huff.o

clean:
	rm -f huff *.o *.h4k
.PHONY: clean
