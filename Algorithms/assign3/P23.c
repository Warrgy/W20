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
#define maxmax(x,y) x>y?x:y

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
        if (strncmp(kString, userInput + i, strlen(kString)) == 0) {
            if (len - i - strlen(kString) != 0) {
                return len - i - strlen(kString);
            }
        }
    }
    return -1;
}

//Check from left to right if the sub string is in the input and where
int getSubResults(char* sub, char* input) {
    for (int i = 0; i < strlen(input) - strlen(sub); i++) {
        if (strncmp(sub, input + i, strlen(sub)) == 0) {
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
    for (int i = strlen(subString) - 1; i >= 0; i--) {
        strcpy(sub, subString + i);

        cur = getSubResults(sub, input);
        if (cur > max) {
            max = cur;
        }
    }
    // free(sub);
    if (max < strlen(input) && max > 0) {
        return max;
    }
    //If case 2 failed, then notify to move length of string - 1.
    return strlen(input) - 1;
}

//Will create a good suffix table needed for the algorithm
int* createBoyerTable(char* input) {
    int len = strlen(input);
    char* string = malloc(sizeof(char) * 50);
    int lenTemp = 0;

    int* table = malloc(len * sizeof(char));

    for (int i = len - 1; i > 0; i--) {
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

//Will get the k length for the two strings. (i.e the length from right to left that match both strings)
int checkSubLength(char* userInput, char* line, int index) {
    int k = 0;
    for (int i = strlen(userInput) - 1; i >= 0; i--) {
        if (userInput[i] == line[index - strlen(userInput) + i + 1]) {
            k++;
        } else {
            break;
        }
    }
    return k;
}

//Will count all the matches in the algorithm
unsigned int countBoyerMooreMatches(char* userInput, char* line, unsigned int* patternShifts) {
    unsigned int count = 0;

    int* badSuffixTable = stringToTable(userInput);
    int* goodSuffix = createBoyerTable(userInput);

    int inputLen = strlen(userInput);

    for (int i = inputLen - 1; i < strlen(line);) {
        int k = checkSubLength(userInput, line, i);
        if (k == inputLen) {
            count++;
            i++;
        } else {
            int offset1 = maxmax(badSuffixTable[intTableIndex(line[i - k])] - k, 1);
            if (k == 0) {
                i += offset1;
            } else {
                i += maxmax(offset1, goodSuffix[k - 1]);
            }
        }

        (*patternShifts)++;
    }

    freeTable(goodSuffix);
    freeTable(badSuffixTable);
    return count;
}

//Count all the matches using the boyer moore algorithm
unsigned int BoyerMooreStringMatching(unsigned int* patternShifts) {
    unsigned int matches = 0;

    char* userInput = getInput();
    FILE* fp = fopen("data_5.txt", "r");
    char* line = malloc(sizeof(char) * MAX_LENGTH + 1);

    //Loop through the 44049 lines in the file, and check each line for matches with the user's input.
    for(int i = 0; i < 44049; i++) {
        if (fgets(line, MAX_LENGTH, fp) == NULL) {
            break;
        }
        matches += countBoyerMooreMatches(userInput, line, patternShifts);
    }


    free(userInput);
    free(line);
    fclose(fp);
    return matches;
}