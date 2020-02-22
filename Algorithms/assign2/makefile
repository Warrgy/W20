CC = gcc
CFLAGS = -Wall -std=c11 -g -lm

all: together

together:  P11 P12 P22 P21 main
	$(CC) $(CFLAGS) main.o P11.o P12.o P21.o P22.o -o assign2

main: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

P11: P11.c
	$(CC) $(CFLAGS) -c P11.c -o P11.o

P12: P12.c
	$(CC) $(CFLAGS) -c P12.c -o P12.o

P21: P21.c
	$(CC) $(CFLAGS) -c P21.c -o P21.o

P22: P22.c
	$(CC) $(CFLAGS) -c P22.c -o P22.o

run:
	./assign2 2.2

valgrind:
	valgrind --leak-check=full --leak-resolution=med --track-origins=yes --show-leak-kinds=all ./assign2 2.2

clean:
	rm -rf *.o assign2