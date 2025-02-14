CC = gcc
CFLAGS = -Wall -Wextra -g

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c	

huffman.o: heap.o hashmap.o huffman.c
	$(CC) $(CFLAGS) -c huffman.c

main.o: huffman.o main.c
	$(CC) $(CFLAGS) -c main.c

all: main.o
	$(CC) $(CFLAGS) -o huff heap.o hashmap.o huffman.o main.o

clean:
	rm -f huff heap.o hashmap.o huffman.o main.o
