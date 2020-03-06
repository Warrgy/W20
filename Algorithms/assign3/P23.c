/**
 * Assignment 3. Part 2.3
 * Boyer-Moore String Matching
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256
#define TABLE_LENGTH 52

char* getInput();
char loopIndexToChar(int i);
void printTable(int* table, int len);
void freeTable(int* table);
int* stringToTable(char* string);
int intTableIndex(char c);

void printBoyerTable(int* table, int len) {
    for(int i = 0; i < len; i++) {
        printf("[%d] -> %d\n", i, table[i]);
    }
}

//Case 1: the substring is located in another part of the input.
int getResultTableBoyer(char* kString, char* userInput) {
    int len = strlen(userInput);

    for (int i = len - strlen(kString); i >= 0; i--) {
        printf("comparing: (%s) & (%s) {w/ len = %ld}\n", kString, userInput + i, strlen(kString));
        if (strncmp(kString, userInput + i, strlen(kString)) == 0) {
            if (len - i - strlen(kString) != 0) {
                printf("returning: %d - %d - %ld = %ld\n", len, i ,strlen(kString), len - i - strlen(kString));
                return len - i - strlen(kString);
            }
        }
    }
    return -1;
}

int getSubResults(char* sub, char* input) {
    for (int i = 0; i < strlen(input) - strlen(sub); i++) {
        printf("comparing %s & %s {w/ len = %ld}\n", sub, input + i, strlen(sub));
        if (strncmp(sub, input + i, strlen(sub)) == 0) {
            printf("returning: %ld - %d - 1 = %ld\n", strlen(input),i,strlen(input) - i - 1);
            return strlen(input) - i - strlen(sub);
        }
    }
    return -1;
}

//Case 2: calculate find results in the substring to satisfy equation.
int checkSubMatches(char* subString, char* input) {
    int cur = 0;
    int max = 0;
    char* sub = malloc(sizeof(char) * strlen(subString) + 1);
    printf("CASE 2: args = (%s,%s)\n", subString, input);
    for (int i = strlen(subString) - 1; i >= 0; i--) {
        strcpy(sub, subString + i);
        printf("passing args(%s, %s)\n", sub, input);
        cur = getSubResults(sub, input);
        if (cur > max) {
            max = cur;
        }
    }
    if (max < strlen(input) && max > 0) {
        printf("returning the max: %d\n",max);
        return max;
    }
    //If case 2 failed, then notify to move length of string - 1.
    printf("returning: %ld {len of string}\n", strlen(input) - 1);
    return strlen(input) - 1;
}

int* createBoyerTable(char* input) {
    int len = strlen(input);
    char* string = malloc(sizeof(char) * 50);
    int lenTemp = 0;

    int* table = malloc(len * sizeof(char));

    for (int i = len - 1; i > 0; i--) {
        printf("copying %s\n", input + i);
        strcpy(string, input + i);
        table[lenTemp] = getResultTableBoyer(string, input);
        //If case 1 failed. run case 2
        if (table[lenTemp] < 0) {
            table[lenTemp] = checkSubMatches(string, input);
        }
        lenTemp++;
    }

    free(string);
    return table;
}

unsigned int countBoyerMooreMatches(char* userInput, char* line, unsigned int* patternShifts) {
    int* badSuffixTable = stringToTable(userInput);
    int* goodSuffix = createBoyerTable(userInput);

    printBoyerTable(goodSuffix, strlen(userInput) - 1);

    freeTable(goodSuffix);
    freeTable(badSuffixTable);
    return 0;
}

unsigned int BoyerMooreStringMatching(unsigned int* patternShifts) {
    unsigned int matches = 0;

    char* userInput = getInput();
    FILE* fp = fopen("data_5.txt", "r");
    char* line = malloc(sizeof(char) * MAX_LENGTH + 1);

    //Loop through the 44049 lines in the file, and check each line for matches with the user's input.
    // for(int i = 0; i < 44049; i++) {
    //     if (fgets(line, MAX_LENGTH, fp) == NULL) {
    //         break;
    //     }
    //     matches += countBoyerMooreMatches(userInput, line, patternShifts);
    // }
    countBoyerMooreMatches(userInput, line, patternShifts);

    free(userInput);
    free(line);
    fclose(fp);
    return matches;
}