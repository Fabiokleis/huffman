CC = gcc
CFLAGS = -Wall -Wextra -g

fila.c:
	$(CC) $(CFLAGS) -c fila.c

hashmap.c: 
	$(CC) $(CFLAGS) -c hashmap.c	

huffman.o: fila.c hashmap.c huffman.c
	$(CC) $(CFLAGS) -c huffman.c

main.o: huffman.c main.c
	$(CC) $(CFLAGS) -c main.c

all: fila.o hashmap.o huffman.o main.o
	$(CC) $(CFLAGS) -o huff fila.o hashmap.o huffman.o main.o

clean:
	rm -f huff fila.o hashmap.o huffman.o main.o
