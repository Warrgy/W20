/**
 * Assignment 3. Part 2.1
 * Brute Force String Matching
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

char* getInput();

//Count the number of matches and patternShifts on the current line
unsigned int countBruteMatches(char* userInput, char* line, unsigned int* patternShifts) {
    unsigned int count = 0;
    //traverse through entire line (keeping in mind for userInput size)
    if (strlen(line) > strlen(userInput))
        for (int i = 0; i < (strlen(line) - strlen(userInput)); i++) {
            //check if the string matches the string at the current instance in the string
            unsigned int lenChecker = 0;
            for (int k = 0; k < strlen(userInput); k++) {
                //if the current two characters matches, then increment length checker
                if (userInput[k] == line[i+k]) {
                    lenChecker++;
                }
            }
            
            //if the amount of matches is equal to the length of the userInput, then increment match count.
            if (lenChecker == strlen(userInput)) {
                count++;
            }
            (*patternShifts)++;
        }
    return count;
}

//Check the amount of total matches and pattern shifts in the file.
unsigned int bruteForceStringMatching(unsigned int* patternShifts) {
    unsigned int count = 0;

    char* userInput = getInput();
    FILE* fp = fopen("data_5.txt", "r");
    char* line = malloc(sizeof(char) * MAX_LENGTH + 1);

    //Loop through the 44049 lines in the file, and check each line for matches with the user's input.
    for(int i = 0; i < 44049; i++) {
        if (fgets(line, MAX_LENGTH, fp) == NULL) {
            break;
        }
        count += countBruteMatches(userInput, line, patternShifts);
    }

    free(userInput);
    free(line);
    fclose(fp);
    return count;
}