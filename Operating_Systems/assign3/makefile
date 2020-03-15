CC = gcc
CFLAGS =  -Wall -g -std=c99 

all: main.c
	$(CC) $(CFLAGS) main.c -o virmem

run:
	./virmem addresses.txt

valgrind:
	valgrind --leak-check=full --leak-resolution=med --track-origins=yes --show-leak-kinds=all ./virmem addresses.txt