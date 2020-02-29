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

#define MAX_LENGTH 256

char* getInput();

//Will return the number of maches. As well as the number of patternShifts
unsigned int countHorspoolMatches(char* userInput, char* line, unsigned int* patternShifts) {
    return 0;
}

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