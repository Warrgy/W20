CC = gcc
CFLAGS = -Wall -std=c11 -g

BIN = ./bin/
SRC = ./src/
INC = ./include/

liblist: $(BIN)LinkedListAPI.o
	$(CC) -shared $(BIN)LinkedListAPI.o -o $(BIN)liblist.so 

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c
	$(CC) $(CFLAGS) -c $(SRC)LinkedListAPI.c -o $@

clean: 
	rm -rf $(BIN)*.o $(BIN)*.so




