/**
 * Assignment 4. Question 1.
 * Dynamic Programming Optimal BST
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Apr 6, 2020
 **/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define WORD_AMOUNT 2045

char* getInput();

unsigned int uniqueWordKeysGlobal = 0;

typedef struct probabilityNode{
    char* key;
    unsigned int occurence;
    float probability;
} searchNode;

int compare(const void* a, const void *b) {
    searchNode* x = (searchNode*) a;
    searchNode* y = (searchNode*) b;
    // printf("comparing %f & %f\n", x->probability , y->probability);
    return y->probability - x->probability;
}

void freeProbabilityTable(searchNode* table) {
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        free(table[i].key);
    }
    free(table);
}

void printProbabilityTable(searchNode* table) {
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        printf("[%d] {%s} -> %d -> %.3f\n", i, table[i].key, table[i].occurence, table[i].probability);
    }
}

//Returns false if the table has the word in it already(and increment occurence), else return true
bool checkUniqueness(char* word, searchNode* table) {
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        if (strcmp(word, table[i].key) == 0) {
            (table[i].occurence)++;
            return false;
        }
    }
    return true;
}

searchNode* createProbabilityTable(char** words) {
    searchNode* table = malloc(sizeof(searchNode));

    //Insert all the unique items in the table
    for (int i = 0; i < WORD_AMOUNT; i++) {
        //check words at position i (also increment occurence)
        if (checkUniqueness(words[i], table)) {
            //if not inside, then add to end
            uniqueWordKeysGlobal++;
            table = realloc(table, sizeof(searchNode) * uniqueWordKeysGlobal);
            (table[uniqueWordKeysGlobal - 1]).key = malloc(sizeof(char) * (strlen(words[i]) + 1));
            strcpy((table[uniqueWordKeysGlobal - 1]).key, words[i]);
            (table[uniqueWordKeysGlobal - 1]).occurence = 1;
            (table[uniqueWordKeysGlobal - 1]).probability = 0.0;
        }
    }

    //Update the probability of each
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        table[i].probability = (float) table[i].occurence / (float) uniqueWordKeysGlobal;
    }

    return table;
}

float** createMainTable() {
    float** table = malloc(sizeof(float*) * (uniqueWordKeysGlobal + 2));
    for (int i = 0; i < uniqueWordKeysGlobal + 2; i++) {
        table[i] = malloc(sizeof(float) * uniqueWordKeysGlobal + 1);
        for (int j = 0; j < uniqueWordKeysGlobal + 1; j++) {
            table[i][j] = 69.0;
        }
    }
    return table;
}

int** createRootTable() {
    int** table = malloc(sizeof(int*) * (uniqueWordKeysGlobal + 2));
    for (int i = 0; i < uniqueWordKeysGlobal + 2; i++) {
        table[i] = malloc(sizeof(int) * uniqueWordKeysGlobal + 1);
        for (int j = 0; j < uniqueWordKeysGlobal + 1; j++) {
            table[i][j] = -1;
        }
    }
    return table;
}

void freeTable(void** table) {
    for (int i = 0; i < uniqueWordKeysGlobal + 2; i++) {
        free(table[i]);
    }
    free(table);
}

void printFloatTable(float** table) {
    printf("   ");
    for (int i = 0; i < uniqueWordKeysGlobal + 1; i++) {
        printf("%d       ", i);
    }
    printf("\n");
    for (int i = 1; i < uniqueWordKeysGlobal + 2; i++) {
        printf("%d| ", i);
        for (int j = 0; j < uniqueWordKeysGlobal + 1; j++) {
            printf("%.3f | ", table[i][j]);
        }
        printf("\n");
    }
}

void printIntTable(int** table) {
    printf("   ");
    for (int i = 0; i < uniqueWordKeysGlobal + 1; i++) {
        printf("%d   ", i);
    }
    printf("\n");
    for (int i = 1; i < uniqueWordKeysGlobal + 2; i++) {
        printf("%d| ", i);
        for (int j = 0; j < uniqueWordKeysGlobal + 1; j++) {
            printf("%d | ", table[i][j]);
        }
        printf("\n");
    }
}

searchNode* testProbability() {
    searchNode* table = malloc(sizeof(searchNode) * 4);
    uniqueWordKeysGlobal = 4;
    for (int i = 0; i < 4; i++) {
        table[i].key = malloc(sizeof(char) * 2);
        strcpy(table[i].key, "A");
        table[i].key[0] = 'A' + i;
    }
    table[0].probability = 0.1;
    table[1].probability = 0.2;
    table[2].probability = 0.4;
    table[3].probability = 0.3;
    return table;
}

void OptimalDynamicBST(char** words) {
    searchNode* probabilityTable = createProbabilityTable(words);
    // searchNode* probabilityTable = testProbability();
    unsigned int n = uniqueWordKeysGlobal;

    float** mainTable = createMainTable();
    int** rootTable = createRootTable();

    for (int i = 1; i < n + 1; i++) {
        mainTable[i][i - 1] = 0.0;
        mainTable[i][i] = probabilityTable[i - 1].probability;
        rootTable[i][i] = i;
    }
    mainTable[n + 1][n] = 0;

    for (unsigned int d = 1; d < n; d++) {
        for (unsigned int i = 1; i < n - d + 1; i++) {
            unsigned int j = i + d;
            float minval = 999999;
            int kmin = 99999;
            for (unsigned int k = i; k < j + 1; k++) {
                if ((mainTable[i][k-1] + mainTable[k + 1][j]) < minval) {
                    minval = mainTable[i][k-1] + mainTable[k + 1][j];
                    kmin = k;
                }
            }
            rootTable[i][j] = kmin;
            float sum = mainTable[i][i]; 
            for (unsigned int s = i + 1; s < j + 1; s++) {
                sum += probabilityTable[s - 1].probability;
            }
            mainTable[i][j] = minval + sum;
        }
    }

    printf(" C[1, n], R ==== %f, %d\n", mainTable[1][n], rootTable[1][n]);

    // printf("[132] {%s} %d -> %f\n", probabilityTable[132].key, probabilityTable[132].occurence, probabilityTable[132].probability);

    // printFloatTable(mainTable);
    // printIntTable(rootTable);

    freeTable((void**)mainTable);
    freeTable((void**)rootTable);
    freeProbabilityTable(probabilityTable);
}