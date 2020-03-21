/* 
 * Assignment 3
 * 
 * Name: Cameron Fisher
 * ID: 1057347
 * Date: Mar 24 / 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH 1000
#define LINE_LENGTH 100

typedef struct TLBRecord {
    int pageNumber;
    int frameNumber;
    int order;
} TLB;

int getOffset(int num) {
    return num & 0xFF;
}

int getPageNum(int num) {
    return num >> 8;
}

//Initialize Page Table
int* initializePageTable() {
    int* table = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        table[i] = -1000;
    }
    return table;
}

//Initialize Physical Memory
int* initializePhysicalMemory() {
    int* table = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        table[i] = -1000;
    }
    
    return table;
}

//Initialize TLB Table
TLB* initializeTLB() {
    TLB* table = malloc(sizeof(TLB) * 16);
    for (int i = 0; i < 16; i++) {
        table[i].pageNumber = -1000;
        table[i].frameNumber = -1000;
        table[i].order = -1000;
    }
    return table;
}

//Get the byte value in location num inside the file BACKING_STORE.bin
int getValue(int num) {
    FILE* fptr = fopen("BACKING_STORE.bin", "r");
    if (fptr == NULL) {
        return -1000;
    }

    fseek(fptr, num, 0);
    char c = fgetc(fptr);
    fclose(fptr);
    return c;
}

//Add the number to the physical memory if not already added.
int addNumToPhysicalMemory(int num, int* physicalMemory) {
    int pageNum = getPageNum(num);

    for (int i = 0; i < 256; i++) {
        if (pageNum == physicalMemory[i]) {
            return i;
        } else if (physicalMemory[i] == -1000) {
            physicalMemory[i] = pageNum;
            return i;
        }
    }
    return -1;
}

//Add the number's frame to the page table and update physical memory
int addToPageTable(int num, int* pageTable, int* physicalMemory) {
    int pageNum = getPageNum(num);
    //Not set yet
    if (pageTable[pageNum] == -1000) {
        int frame = addNumToPhysicalMemory(num, physicalMemory);
        if (frame == -1) {
            //error occured
            return 0;
        }
        pageTable[pageNum] = frame;
        return 1;
    } 
    return 0;
}

//Get the corresponding frame for the pageNumber
int getFrame(int pageNum, int* pageTable) {
    return pageTable[pageNum];
}

//Will increment the order of everything in the table by 1
void incrementCount(TLB* table) {
    for (int i = 0; i < 16; i++) {
        (table[i].order)++;
    }
}

//Return frame number corresponding to the pageNumber
int checkTLB(int pageNum, TLB* TLBTable, int* pageTable, unsigned int* TLBHitCount) {
    //Check table if it is in it already. But if there is an empty spot. Then add it in that spot.
    for (int i = 0; i < 16; i++) {
        if (TLBTable[i].pageNumber == pageNum) {
            //hit
            (*TLBHitCount)++;
            return TLBTable[i].frameNumber;
        } else if (TLBTable[i].pageNumber == -1000) {
            //Free empty spot
            TLBTable[i].pageNumber = pageNum;
            TLBTable[i].frameNumber = getFrame(pageNum, pageTable);
            TLBTable[i].order = 0;

            incrementCount(TLBTable);
            return TLBTable[i].frameNumber;
        }
    }

    unsigned int max = 0;
    unsigned int maxIndex = 0;
    //Swap with the oldest page
    for (int i = 0; i < 16; i++) {
        if (TLBTable[i].order > max) {
            max = TLBTable[i].order;
            maxIndex = i;
        }
    }
    TLBTable[maxIndex].pageNumber = pageNum;
    TLBTable[maxIndex].frameNumber = getFrame(pageNum, pageTable);
    TLBTable[maxIndex].order = 0;

    incrementCount(TLBTable);

    return getFrame(pageNum, pageTable);
}

//Check the number (num) for the frame number first with a TLBTable then the pageTable. Return the amount of TLBHits
unsigned int manageMemory(int num, TLB* TLBTable, int* pageTable, int* physicalMemory) {
    unsigned int TLBHitCount = 0;

    int pageNum = getPageNum(num);
    int offset = getOffset(num);

    int value = getValue(num);
    if (value == -1000) {
        return 0;
    }

    int frame = checkTLB(pageNum, TLBTable, pageTable, &TLBHitCount);

    int physicalAddress = frame * 256 + offset;

    printf("Virtual address: %d Physical address: %d Value: %d\n", num, physicalAddress, value);
    
    return TLBHitCount;
}

int main(int argc, char **argv) {
    char line[LINE_LENGTH];

    if (argc != 2) {
        printf("Do ./virmem {file}\n");
        return -1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Issue opening file: %s\n", argv[1]);
        return -1;
    }

    unsigned int pageFault = 0;
    unsigned int TLBHitCount = 0;
    unsigned int addressCount = 0;

    //Initialize tables
    int* pageTable = initializePageTable();
    TLB* TLBTable = initializeTLB();
    int* physicalMemory = initializePhysicalMemory();

    //Get each number from file 1 by 1 and check each one in the page table and memory management
    for (int i = 0; i < FILE_LENGTH; i++) {
        if (fgets(line, LINE_LENGTH, fp) == NULL) {
            break;
        }
        int num = atoi(line);
        
        //Add result into page table
        pageFault += addToPageTable(num, pageTable, physicalMemory);
        //Find the virtual addreess
        TLBHitCount += manageMemory(num, TLBTable, pageTable, physicalMemory);
        
        addressCount++;
    }

    printf("Number of Translated Addresses = %d\n", addressCount);
    printf("Page Faults = %d\n", pageFault);
    printf("Page Fault Rate = %.3f\n", (float)pageFault / (float)addressCount);
    printf("TLB Hits = %d\n", TLBHitCount);
    printf("TLB Hit Rate = %.3f\n", (float)TLBHitCount / (float)addressCount);

    free(physicalMemory);
    free(pageTable);
    free(TLBTable);
    fclose(fp);

    return 0;
}