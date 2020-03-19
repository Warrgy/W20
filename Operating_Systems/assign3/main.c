#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH 1000
#define LINE_LENGTH 100

typedef struct TLBRecord {
    int pageNumber;
    int frameNumber;
} TLB;

int getOffset(int num) {
    return num & 0xFF;
}

int getPageNum(int num) {
    return num >> 8;
}

void freePhysicalMemory(int* table) {
    free(table);
}

int* initializePageTable() {
    int* table = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        table[i] = -1000;
    }
    return table;
}

int* initializePhysicalMemory() {
    int* table = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        table[i] = -1000;
    }
    
    return table;
}

TLB* initializeTLB() {
    TLB* table = malloc(sizeof(TLB) * 16);
    for (int i = 0; i < 16; i++) {
        table[i].pageNumber = -1;
        table[i].frameNumber = -1;
    }
    return table;
}

// int getFrameFromPageTable(int* table, int pageNum) {
//     return table[pageNum];
// }

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

void addToPageTable(int num, int* pageTable, int* physicalMemory) {
    int pageNum = getPageNum(num);
    //Not set yet
    if (pageTable[pageNum] == -1000) {
        int frame = addNumToPhysicalMemory(num, physicalMemory);
        if (frame == -1) {
            //error occured
            return;
        }
        pageTable[pageNum] = frame;
    } 
}

void addPage(int num, int* pageTable, int* physicalMemory) {
    addToPageTable(num, pageTable, physicalMemory);
    // int frame = getFrameViaPageTable(num, pageTable);
    
}

int getFrame(int pageNum, TLB* TLBTable, int* pageTable) {
    return pageTable[pageNum];
}

int manageMemory(int num, TLB* TLBTable, int* pageTable, int* physicalMemory, unsigned int* TLBHitCount) {
    int pageNum = getPageNum(num);
    int offset = getOffset(num);
    int count = 0;

    int value = getValue(num);
    if (value == -1000) {
        return 0;
    }

    int frame = getFrame(pageNum, TLBTable, pageTable);

    int physicalAddress = frame * 256 + offset;

    printf("Virtual Address: %d Physical Address: %d Value = %d\n", num, physicalAddress, value);

    return count;
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

    int* pageTable = initializePageTable();
    TLB* TLBTable = initializeTLB();
    int* physicalMemory = initializePhysicalMemory();

    for (int o = 0; o < 2; o++) {       //Run twice. once for making page table, then once more for using it
        for (int i = 0; i < FILE_LENGTH; i++) {
            if (fgets(line, LINE_LENGTH, fp) == NULL) {
                break;
            }
            int num = atoi(line);

            //Step 1: Add virtual addresses to tables
            if (o == 0) {
                addPage(num, pageTable, physicalMemory);
            //Step 2: Manage memory
            } else {
                pageFault += manageMemory(num, TLBTable, pageTable, physicalMemory, &TLBHitCount);
                addressCount++;
            }
        }
        fseek(fp, 0, 0);
    }

    printf("Number of Translated Addresses = %d\n", addressCount);
    printf("Page Faults = %d\n", pageFault);
    printf("Page Fault Rate = %d\n", pageFault / addressCount);
    printf("TLB Hits = %d\n", TLBHitCount);
    printf("TLB Hit Rate = %d\n", TLBHitCount / addressCount);

    freePhysicalMemory(physicalMemory);
    free(pageTable);
    free(TLBTable);
    fclose(fp);
}