/**
 * Assignment 4. 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 30, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define WORD_AMOUNT 2045
#define LINE_AMOUNT 204

void OptimalDynamicBST(char** words);
void OptimalGreedyBST(char** words);

//Free the word array
void free2DArray(char** words) {
    for (int i = 0; i < WORD_AMOUNT; i++) {
        if (words[i] == NULL) {
            break;
        }
        free(words[i]);
    }
    free(words);
}

//Will print all the words in the array
void printWords(char** words) {
    for (int i = 0; i < WORD_AMOUNT; i++) {
        if (words[i] == NULL) {
            break;
        }
        printf("[%d] {%s}\n", i, words[i]);
    }
}

//Will get the key from the user
char* getInput() {
    char input[100];
    printf("Enter a key: ");
    scanf("%s", input);

    char* key = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(key, input);
    return key;
}

//Will parse the file data_7.txt and extract all words inside it.
char** parseFile() {
    FILE *fp = fopen("data_7.txt", "r");
    if (fp == NULL) {
        printf("Unable to find file data_7.txt\n");
        return NULL;
    }

    char** words = malloc(sizeof(char*) * WORD_AMOUNT);
    unsigned int wordCount = 0;
    char line[256];

    for (int i = 0; i < LINE_AMOUNT; i++) {
        if (fgets(line, 256, fp) == NULL) {
            break;
        }

        char c;
        char prev = '\0';
        //Go through the line and get the words, character by character
        for (int j = 0; j < 256; j++) {
            c = line[j];

            if (c == '\n') {
                break;
            }

            //If character then use in word
            if (islower(c) || isupper(c)) {
                if (!isupper(prev) && !islower(prev)) {
                    //start new word
                    wordCount++;
                    words[wordCount - 1] = malloc(sizeof(char) * 2);
                    words[wordCount - 1][0] = c;
                    words[wordCount - 1][1] = '\0';
                } else {
                    //add onto the word
                    int len = strlen(words[wordCount - 1]);
                    words[wordCount - 1] = realloc(words[wordCount - 1], len + 2);
                    words[wordCount - 1][len] = c;
                    words[wordCount - 1][len + 1] = '\0';
                }
                
            } 
            prev = c;
        }
    }
    fclose(fp);
    return words;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please do ./assign4 {1,2}\n");
        return -1;
    }

    clock_t start;

    char** words = parseFile();
    if (words == NULL) {
        return -1;
    }

    //Dynamic Programming
    if (strcmp(argv[1],"1") == 0) {
        start = clock();

        OptimalDynamicBST(words);

    //Greedy Technique
    } else if (strcmp(argv[1], "2") == 0) {
        start = clock();

        OptimalGreedyBST(words);

    } else {
        printf("Please enter one of the following next time: {1,2}\n");
        free2DArray(words);
        return -1;
    }

    free2DArray(words);

    clock_t end = clock();
    double totalTime = (double)(end-start) / CLOCKS_PER_SEC;
    printf("\nAlgorithm time = %lfs\n", totalTime);

    return 0;
}