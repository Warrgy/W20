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

void freePhysicalMemory(int* table) {
    free(table);
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

//Add page to pagetable
void addPage(int num, int* pageTable, int* physicalMemory) {
    addToPageTable(num, pageTable, physicalMemory);
    // int frame = getFrameViaPageTable(num, pageTable);
    
}


int getFrame(int pageNum, int* pageTable) {
    return pageTable[pageNum];
}

void incrementCount(TLB* table) {
    for (int i = 0; i < 16; i++) {
        (table[i].order)++;
    }
}

//Return frame number corresponding to the pageNumber
int checkTLB(int pageNum, TLB* TLBTable, int* pageTable, unsigned int* TLBHitCount, unsigned int* pageFaultsCount) {
    printf("Checking TLB\t");
    //Check table if it is in it already. But if there is an empty spot. Then add it in that spot.
    for (int i = 0; i < 16; i++) {
        if (TLBTable[i].pageNumber == pageNum) {
            //hit
            printf("Incrementing hit.\n");
            (*TLBHitCount)++;
            return TLBTable[i].frameNumber;
        } else if (TLBTable[i].pageNumber == -1000) {
            printf("incrementing the pageFault\n");
            (*pageFaultsCount)++;
            TLBTable[i].pageNumber = pageNum;
            TLBTable[i].frameNumber = getFrame(pageNum, pageTable);
            TLBTable[i].order = 0;
            incrementCount(TLBTable);
            return TLBTable[i].frameNumber;
        }
    }
    
    
    printf("Full, so checking..\t");

    //Swap with the oldest page
    for (int i = 0; i < 16; i++) {
        if (TLBTable[i].order == 15) {
            printf("incrementing pageFault\n");
            (*pageFaultsCount)++;
            TLBTable[i].pageNumber = pageNum;
            TLBTable[i].frameNumber = getFrame(pageNum, pageTable);
            TLBTable[i].order = 0;
        }
    }
    printf("Swapped out val.");
    incrementCount(TLBTable);

    //TESTING
    for (int i = 0; i < 16; i++) {
        printf("[%d] %d\n", i , TLBTable[i].order);
    }

    return getFrame(pageNum, pageTable);
}

unsigned int manageMemory(int num, TLB* TLBTable, int* pageTable, int* physicalMemory, unsigned int* TLBHitCount) {
    int pageNum = getPageNum(num);
    int offset = getOffset(num);
    unsigned int pageFaults = 0;

    int value = getValue(num);
    if (value == -1000) {
        return 0;
    }

    int frame = checkTLB(pageNum, TLBTable, pageTable, TLBHitCount, &pageFaults);

    int physicalAddress = frame * 256 + offset;

    printf("Virtual address: %d Physical address: %d Value: %d\n", num, physicalAddress, value);

    return pageFaults;
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

    for (int i = 0; i < FILE_LENGTH; i++) {
        if (fgets(line, LINE_LENGTH, fp) == NULL) {
            break;
        }
        int num = atoi(line);

        
        addPage(num, pageTable, physicalMemory);
        pageFault += manageMemory(num, TLBTable, pageTable, physicalMemory, &TLBHitCount);
        
        addressCount++;
    }

    // for (int i = 0; i < 256; i++) {
    //     printf("   [%d] %d\t%d\n", i,pageTable[i], physicalMemory[i]);
    // }

    printf("Number of Translated Addresses = %d\n", addressCount);
    printf("Page Faults = %d\n", pageFault);
    printf("Page Fault Rate = %.3f\n", (float)pageFault / (float)addressCount);
    printf("TLB Hits = %d\n", TLBHitCount);
    printf("TLB Hit Rate = %.3f\n", (float)TLBHitCount / (float)addressCount);

    freePhysicalMemory(physicalMemory);
    free(pageTable);
    free(TLBTable);
    fclose(fp);

    return 0;
}