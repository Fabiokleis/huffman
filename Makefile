CC = gcc
CFLAGS = -Wall -Wextra -g -lm

all: huff
.PHONY: all

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

huff: main.o
	$(CC) $(CFLAGS) -o huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o

clean:
	rm -f huff heap.o hashmap.o huffman.o kmeans.o stack.o arvore.o main.o *.h4k
