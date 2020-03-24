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


void OptimalDynamicBST(char** words) {
    searchNode* probabilityTable = createProbabilityTable(words);

    printProbabilityTable(probabilityTable);

    printf("Sup dude.\n");
    
    freeProbabilityTable(probabilityTable);
}