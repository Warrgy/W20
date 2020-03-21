/**
 * Assignment 4. 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Apr 6, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define WORD_AMOUNT 2045
#define LINE_AMOUNT 204

void free2DArray(char** words) {
    for (int i = 0; i < WORD_AMOUNT; i++) {
        free(words[i]);
    }
    free(words);
}

void printWords(char** words) {
    for (int i = 0; i < WORD_AMOUNT; i++) {
        printf("[%d] %s\n", i, words[i]);
    }
}

char** parseFile() {
    FILE *fp = fopen("data_7.txt", "r");
    if (fp == NULL) {
        printf("Unable to find file data_7.txt\n");
        exit(-1);
    }

    char** words = malloc(sizeof(char*) * WORD_AMOUNT);
    unsigned int wordCount = 0;
    char line[256];

    for (int i = 0; i < 1; i++) {
        //Get next word on line
        if (fgets(line, 256, fp) == NULL) {
            break;
        }
        printf("line = %s\n", line);

        words[wordCount] = malloc(1);
        char c;
        c = fgetc(fp);
        for (int j = 0; j < 256; j++) {
            if (c != ' ' && c != '\n' && c != '\0') {
                words[wordCount] = realloc(words[wordCount], strlen(words[wordCount]) + 2);
                words[wordCount][strlen(words[wordCount])] = c;
                words[wordCount][strlen(words[wordCount]) + 1] = '\0';
            } 
            c = fgetc(fp);

        }


        words[i] = malloc(1);
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
    printWords(words);
    free2DArray(words);

    //Dynamic Programming
    if (strcmp(argv[1],"1") == 0) {

        start = clock();

    //Greedy Technique
    } else if (strcmp(argv[1], "2") == 0) {

        start = clock();

    } else {
        printf("Please enter one of the following next time: {1,2}\n");
        return -1;
    }

    clock_t end = clock();
    double totalTime = (double)(end-start) / CLOCKS_PER_SEC;
    printf("\nAlgorithm time = %lfs\n", totalTime);

    return 0;
}