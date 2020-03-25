/**
 * Assignment 4. Question 2.
 * Greedy Technique Optimal BST
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Apr 6, 2020
 **/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define WORD_AMOUNT 2045

typedef struct probabilityNode{
    char* key;
    unsigned int occurence;
    float probability;
} searchNode;

typedef struct TreeRoot {
    char* key;
    float probability;
    struct TreeRoot* child[2];
} Tree;

char* getInput();
searchNode* createProbabilityTable(char** words);
void printProbabilityTable(searchNode* table);
void freeProbabilityTable(searchNode* table);
unsigned int getUniqueWordsCount();
float getProbability(searchNode* probabilityTable, char* key);

Tree* initTree(char* key, float probability) {
    Tree* t = malloc(sizeof(Tree));
    t->probability = probability;
    printf("part\n");
    if (key != NULL) {
        t->key = malloc(sizeof(char) * (strlen(key) + 1));
        strcpy(t->key, key);
    } else {
        t->key = NULL;
    }
    printf("ay\n");

    t->child[0] = NULL;
    t->child[1] = NULL;

    return t;
}

void printTree(Tree* t) {
    if (t != NULL) {
        printf("[%s %f]", t->key, t->probability);
        if (t->child[0] != NULL) {
            printf("go left\n");
            printTree(t->child[0]);
        } 
        if (t->child[1] != NULL) {
            printf("go right\n");
            printTree(t->child[1]);
        }
        printf("End\n");
    }
}

void freeTree(Tree* t) {
    if (t != NULL) {
        free(t->key);
        freeTree(t->child[0]);
        freeTree(t->child[1]);
        free(t);
    }
}

Tree* generateTree(searchNode* table, unsigned int length, unsigned int left, unsigned int right, Tree* parent, bool isRight) {
    if (table == NULL) {
        return NULL;
    }
    unsigned int median = (right - left) / 2;

    printf("sup -> median = %d, left = %d, right = %d\n", median, left, right);
    Tree* node = initTree(table[median].key, table[median].probability);
printf("heya\n");
    if (parent != NULL) {
        if (isRight) {
            //right
            printf("came from right.\n");
            parent->child[1] = node;
        } else {
            //left
            printf("came from left.\n");
            parent->child[0] = node;
        }
    }

    generateTree(table, length, median + 1, right, node, true);
    generateTree(table, length, left, median, node, false);
    
    return node;
}

void searchTree(Tree* t, char* key, searchNode* table) {
    if (table == NULL) {
        return;
    }
    if (t != NULL) {
        float keyProbability = getProbability(table, key);
        if (keyProbability == t->probability) {
            //compare strings
        } 
    }
}

void OptimalGreedyBST(char** words) {
    searchNode* table = createProbabilityTable(words);
    unsigned int n = getUniqueWordsCount();

    // printProbabilityTable(table);

    Tree* t = generateTree(table, n, 0, n - 1, NULL, false);
    if (t == NULL) {
        freeProbabilityTable(table);
        return;
    }

    printTree(t);

    char* key = getInput();

    // searchTree(t, key, table);


    free(key);
    freeTree(t);
    freeProbabilityTable(table);
}