#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH 1000
#define LINE_LENGTH 100

typedef struct TLBRecord {
    int* pageNumber;
    int* frameNumber;
} TLB;

int getOffset(int num) {
    return num & 0xFF;
}

int getPageNum(int num) {
    return num >> 8;
}

int* createPageTable() {
    int* table = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        table[i] = i;
    }
    return table;
}

TLB* createTLB() {
    TLB* table = malloc(sizeof(TLB) * 16);
    return table;
}

int getFrameFromPageTable(int* table, int pageNum) {
    return table[pageNum];
}

int manageMemory(int num, TLB* TLBTable, int* pageTable) {
    int pageNum = getPageNum(num);
    int offset = getOffset(num);

    printf("num %d -> pageNum %d, offset %d\n", num, pageNum,offset);

    int frame = getFrameFromPageTable(pageTable, pageNum);


    return 0;
}

int main(int argc, char **argv) {
    char line[LINE_LENGTH];

    if (argc != 2) {
        printf("Do ./virmem {file}\n");
        return -1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Issue opening file.\n");
        return -1;
    }

    int pageFault = 0;
    int* pageTable = createPageTable();
    TLB* TLBTable = createTLB();

    // for (int i = 0; i < 256; i++) {
    //     printf("table[%d] = %d\n", i, pageTable[i]);
    // }

    for (int i = 0; i < FILE_LENGTH; i++) {
        if (fgets(line, LINE_LENGTH, fp) == NULL) {
            break;
        }
        int num = atoi(line);

        pageFault += manageMemory(num, TLBTable, pageTable);
    }

    FILE* fptr = fopen("BACKING_STORE.bin", "r");

    // for(int i = 0; i < 256; i++) {
    //     char line[256];

    //     fgets(line, 256 , fptr);
    //     printf("line = [%s]\n", line);
    //     for (int k = 0; k < 256; k++) {
    //         int f = line[k];
    //         printf("%d,", f);
    //     }
        
    // }
    fseek(fptr, 1678, 0);
    char c = fgetc(fptr);
    int g = c;
    printf("the val is %d\n", c);
    fclose(fptr);



    free(pageTable);
    free(TLBTable);
    fclose(fp);
}