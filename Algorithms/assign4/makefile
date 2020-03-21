CC = gcc
CFLAGS = -Wall -std=c11 -g -lm
algorithm = 1

all: together

together: main Q1 Q2 
	$(CC) $(CFLAGS) main.o Q1.o Q2.o -o assign4

main: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

Q1: Q1.c
	$(CC) $(CFLAGS) -c Q1.c -o Q1.o

Q2: Q2.c
	$(CC) $(CFLAGS) -c Q2.c -o Q2.o

run: 
	./assign4 $(algorithm)

valgrind:
	valgrind --leak-check=full --leak-resolution=med --track-origins=yes --show-leak-kinds=all ./assign4 $(algorithm)

clean:
	rm -rf *.o assign4

