UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g -lxml2 -I$(INC_PATH)


LDFLAGS= -L.

BIN = ./bin/
SRC = ./src/
INC = ./include/

ifeq ($(UNAME), Linux)
	INC_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	INC_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif


all: $(BIN)SVGParser
	
$(BIN)SVGParser: $(BIN)SVGParser.o $(BIN)libsvgparse.so $(SRC)SVGParser.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)SVGParser $(BIN)*.o #-llist
	
$(BIN)SVGParser.o: $(SRC)SVGParser.c
	$(CC) $(CFLAGS)  -c -fpic $(SRC)SVGParser.c -o $@
	
$(BIN)libsvgparse.so: $(BIN)LinkedListAPI.o
	$(CC) -shared -o $(BIN)libsvgparse.so $(BIN)LinkedListAPI.o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)LinkedListAPI.c -o $@

$(SRC)SVGParser.c: $(SRC)SVGParser.c $(INC)SVGParser.h
	$(CC) $(CFLAGS)  $(SRC)SVGParser.c  -o $(BIN)SVGParser

run: 
	./bin/SVGParser

valgrind: 
	valgrind  --leak-check=full  ./bin/SVGParser

gdb:
	gdb ./bin/SVGParser

clean:
	rm -rf $(BIN)SVGParser xmlExample $(BIN)*.o $(BIN)*.so
