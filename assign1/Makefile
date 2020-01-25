UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g -lxml2 -lm
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


parser: SVGParser
	
SVGParser: $(BIN)SVGParser.o libsvgparse.so $(SRC)SVGParser.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)SVGParser $(BIN)*.o #-llist
	
$(BIN)SVGParser.o: $(SRC)SVGParser.c
	$(CC) $(CFLAGS) -I$(INC) -I$(INC_PATH) -c -fpic $(SRC)SVGParser.c -o $@
	
libsvgparse.so: $(BIN)LinkedListAPI.o
	$(CC) -shared -o $(BIN)libsvgparse.so $(BIN)LinkedListAPI.o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic -I$(INC) $(SRC)LinkedListAPI.c -o $@

$(SRC)SVGParser.c:  $(INC)SVGParser.h
	$(CC) $(CFLAGS)  $(SRC)SVGParser.c  -o $(BIN)SVGParser

run: 
	./bin/SVGParser

valgrind: 
	valgrind  --leak-check=full  ./bin/SVGParser

gdb:
	gdb ./bin/SVGParser

clean:
	rm -rf $(BIN)SVGParser $(BIN)*.o $(BIN)*.so
