CC = gcc
CFLAGS = -Wall -std=c11 -g -lm
algorithm = 1.2

all: together

together: main 11 12 21 22 23
	$(CC) $(CFLAGS) main.o P11.o P12.o P21.o P22.o P23.o -o assign3

main: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

11: P11.c
	$(CC) $(CFLAGS) -c P11.c -o P11.o

12: P12.c
	$(CC) $(CFLAGS) -c P12.c -o P12.o

21: P21.c
	$(CC) $(CFLAGS) -c P21.c -o P21.o

22: P22.c
	$(CC) $(CFLAGS) -c P22.c -o P22.o

23: P23.c
	$(CC) $(CFLAGS) -c P23.c -o P23.o

run: 
	./assign3 $(algorithm)

valgrind: 
	valgrind --leak-check=full --leak-resolution=med --track-origins=yes --show-leak-kinds=all ./assign3 $(algorithm)

clean:
	rm -rf *.o assign3
