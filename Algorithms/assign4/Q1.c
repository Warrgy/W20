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

//Free a probability table
void freeProbabilityTable(searchNode* table) {
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        free(table[i].key);
    }
    free(table);
}

//Print a probability table
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

//Compare function
int compare(const void* a, const void* b) {
    const searchNode* x = (const searchNode*) a;
    const searchNode* y = (const searchNode*) b;

    return strcmp(x->key, y->key);
}

//Create a probability table
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

    qsort(table, uniqueWordKeysGlobal, sizeof(searchNode), compare);

    return table;
}

//Create a main table
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

//Create a root table
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

//Free a table
void freeTable(void** table) {
    for (int i = 0; i < uniqueWordKeysGlobal + 2; i++) {
        free(table[i]);
    }
    free(table);
}

//Print a main table (float table)
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

//Print a root table (int table)
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

//Will return the probability for the given key in the table
float getProbability(searchNode* probabilityTable, char* key) {
    for (int i = 0; i < uniqueWordKeysGlobal; i++) {
        if (strcmp(probabilityTable[i].key, key) == 0) {
            return probabilityTable[i].probability;
        }
    }
    return -10.0;
}

//Get the index of the key from input
unsigned int getKeyIndex(searchNode* probabilityTable, char* key) {
    for (unsigned int i = 0; i < uniqueWordKeysGlobal; i++) {
        if (strcmp(probabilityTable[i].key, key) == 0) {
            return i + 1;
        }
    }
    return (rand() % uniqueWordKeysGlobal) + 1;
}

//Recursively go through tree and find if the key matches
void findKey(float** mainTable, int** rootTable, searchNode* probabilityTable, char* key, unsigned int keyIndex, unsigned int left, unsigned int right) {
    int root = rootTable[left][right];
    printf("Compared with %s (%.3f), ", probabilityTable[root - 1].key, mainTable[left][right]);
    if (keyIndex < root) {
        //left
        printf("go left.\n");
        findKey(mainTable, rootTable, probabilityTable, key, keyIndex, left, root - 1);
    } else if (keyIndex > root) {
        //right
        printf("go right.\n");
        findKey(mainTable, rootTable, probabilityTable, key, keyIndex, root + 1, right);
    } else {
        //compare keys for equality (either not found or found)
        if (strcmp(probabilityTable[root - 1].key, key) == 0) {
            printf("found.\n");
        } else {
            printf("Not found.\n");
        }
    }
}

//Use dynamic programming and create a Optimal binary search tree
void OptimalDynamicBST(char** words) {
    char* key = getInput();

    searchNode* probabilityTable = createProbabilityTable(words);
    unsigned int n = uniqueWordKeysGlobal;

    float** mainTable = createMainTable();
    int** rootTable = createRootTable();

    //Initialize tables
    for (int i = 1; i < n + 1; i++) {
        mainTable[i][i - 1] = 0.0;
        mainTable[i][i] = probabilityTable[i - 1].probability;
        rootTable[i][i] = i;
    }
    mainTable[n + 1][n] = 0;

    //Fill in table
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
    //Use the tables generated find the key.
    findKey(mainTable, rootTable, probabilityTable, key, getKeyIndex(probabilityTable, key), 1, n);

    freeTable((void**)mainTable);
    freeTable((void**)rootTable);
    freeProbabilityTable(probabilityTable);
    free(key);
}