/**
 * Assignment 3. Part 2.2
 * Horspool String Matching
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256
#define TABLE_LENGTH 52

char* getInput();

//Return the corresponding character for the index in the table.
char loopIndexToChar(int i) {
    if (i < 26)
        return 'a' + i;
    else
        return 'A' + i - 26;
}

//Used to print table, for development.
void printTable(int* table, int len) {
    for(int i = 0; i < len; i++) {
        printf("[%c] -> %d\n", loopIndexToChar(i), table[i]);
    }
}

void freeTable(int* table) {
    free(table);
}

//Calculate the distance that the character is from the right
int stringToIntLength(char* string, char c) {
    int count = 0;
    for (int i = strlen(string) - 1; i >= 0; i--) {
        if (string[i] == c)
            break;
        count++;
    }
    if (count == 0) {
        return strlen(string);
    }
    return count;
}

//Convert the string into a table with all alphabetic characters
int* stringToTable(char* string) {
    int* table = malloc(sizeof(int) * TABLE_LENGTH);
    char count = 'a';
    for (int i = 0; i < TABLE_LENGTH; i++) {
        if (count == ('z' + 1))
            count = 'A';
        table[i] = stringToIntLength(string, count);
        count++;
    }
    return table;
}

//Get the integer representation of the index for the table.
int intTableIndex(char c) {
    if (islower(c))
        return c - 'a';
    else if (isupper(c))
        return c - 'A' + 26;
    else 
        return -1;
}

//Will return the number of maches. As well as the number of patternShifts
unsigned int countHorspoolMatches(char* userInput, char* line, unsigned int* patternShifts) {
    unsigned int count = 0;
    int* table = stringToTable(userInput);

    unsigned int inputLen = (strlen(userInput));
    unsigned int lenChecker = 0;
    // printf("checking line: %s\n", line);
    for (int i = inputLen - 1; i < strlen(line); ) {
                // printf("comparing [%c] & [%c] {i = %d}\t", userInput[inputLen - 1], line[i], i);

        int index = intTableIndex(line[i]);
        if (userInput[inputLen - 1] == line[i]) {
            if (table[index] == 0) {
                //check the entire string.
            } else {
                //continue the loop{i +=} {patternShifts}
            }
            lenChecker += table[index];
        } else {
            lenChecker = 0;
        }
        // printf("lenchecker = %d, inputLen = %d\t", lenChecker, inputLen);
        if (lenChecker  == inputLen) {
            // printf("{i = %d}checking for entire string equality...{[%s] & [%s] <- %ld}\n",i, userInput, line + i - strlen(userInput) + 1, strlen(userInput));
            if (strncmp(userInput, (line + i - strlen(userInput) + 1), strlen(userInput)) == 0) {
                // printf("incrementing count.\n");
                count++;
            }
        }
        // printf("got index: %d\n", index);
        if (index >= 0) {
            // printf("incrementing by %d\n", table[index]);
            i += table[index];
        } else {
            // printf("incrementing by default aamount\n");
            i += inputLen;
        }
        (*patternShifts)++;
        
    }

    freeTable(table);
    return count;
}

//Go through entire file and match the requested string to the everything in data_5.txt
unsigned int HorspoolStringMatching(unsigned int* patternShifts) {
    unsigned int matches = 0;

    char* userInput = getInput();
    FILE* fp = fopen("data_5.txt", "r");
    char* line = malloc(sizeof(char) * MAX_LENGTH + 1);

    //Loop through the 44049 lines in the file, and check each line for matches with the user's input.
    for(int i = 0; i < 44049; i++) {
        if (fgets(line, MAX_LENGTH, fp) == NULL) {
            break;
        }
        matches += countHorspoolMatches(userInput, line, patternShifts);
    }

    free(userInput);
    free(line);
    fclose(fp);
    return matches;
}