/**
 * Assignment 4. Question 2.
 * Greedy Technique Optimal BST
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 30, 2020
 **/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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

//Initialize a tree node
Tree* initTree(char* key, float probability) {
    Tree* t = malloc(sizeof(Tree));
    t->probability = probability;

    if (key != NULL) {
        t->key = malloc(sizeof(char) * (strlen(key) + 1));
        strcpy(t->key, key);
    } else {
        t->key = NULL;
    }


    t->child[0] = NULL;
    t->child[1] = NULL;

    return t;
}

//Print the tree
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

//Free the tree
void freeTree(Tree* t) {
    if (t != NULL) {
        free(t->key);
        freeTree(t->child[0]);
        freeTree(t->child[1]);
        free(t);
    }
}

//Will get the most probabale index in the table between left and right indices
unsigned int getMostProbabableIndex(searchNode* table, unsigned int left, unsigned int right) {
    float max = table[left].probability;
    unsigned int maxIndex = left;
    for (unsigned int i = left; i < right ; i++) {
        if (table[i].probability > max) {
            max = table[i].probability;
            maxIndex = i;
        }
    }
    return maxIndex;
}

//Will generate the tree. At each call it will get the most probable index between the indices and make it the node
Tree* generateTree(searchNode* table, unsigned int length, unsigned int left, unsigned int right, Tree* parent, bool isRight) {
    if (table == NULL) {
        return NULL;
    }
    unsigned int maxProbIndex = getMostProbabableIndex(table, left, right);
    if (maxProbIndex == left && maxProbIndex == right) {
        return NULL;
    }

    Tree* node = initTree(table[maxProbIndex].key, table[maxProbIndex].probability);
    if (parent != NULL) {
        if (isRight) {
            //right
            parent->child[1] = node;
        } else {
            //left
            parent->child[0] = node;
        }
    }

    generateTree(table, length, maxProbIndex + 1, right, node, true);
    generateTree(table, length, left, maxProbIndex, node, false);
    
    return node;
}

//Search the tree for the key
void searchTree(Tree* t, char* key) {
    if (t != NULL) {
        printf("Compared with %s (%.3f), ", t->key, t->probability);
        if (strcmp(t->key, key) == 0) {
            printf("found string.\n");
        } else if (strcmp(t->key, key) > 0) {
            printf("go left.\n");
            searchTree(t->child[0], key);
        } else {
            printf("go right.\n");
            searchTree(t->child[1], key);
        }
    } else {
        printf("Not found.\n");
    }
}

//Create an optimal BST using the greedy technique
void OptimalGreedyBST(char** words) {
    searchNode* table = createProbabilityTable(words);
    unsigned int n = getUniqueWordsCount();

    Tree* t = generateTree(table, n, 0, n, NULL, true);
    if (t == NULL) {
        freeProbabilityTable(table);
        return;
    }

    char* key = getInput();

    searchTree(t, key);

    free(key);
    freeTree(t);
    freeProbabilityTable(table);
}