/**
 * Assignment 3. 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** BruteForceAnagram(char** array, unsigned int length, unsigned int* numAnagrams);
char** PreSortAnagram(char** array, unsigned int len, unsigned int* numAnagrams);
unsigned int bruteForceStringMatching(unsigned int* patternShifts);
unsigned int HorspoolStringMatching(unsigned int* patternShifts);
unsigned int BoyerMooreStringMatching(unsigned int* patternShifts);

unsigned int globalArrayLength = 0;

//Will get input. Is used in every algorithm.
char* getInput() {
    char input[100];
    printf("Enter the string: ");
    scanf(" %s ", input);

    char* alloc = malloc(sizeof(char) * strlen(input) + 1);
    strcpy(alloc, input);
    return alloc;
}

//Frees 2D array
void free2DArray(char** arr, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        free(arr[i]);
    }
    free(arr);
}

//Print 2D array
void printArray(char** arr, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        if (arr[i] == NULL) {
            break;
        }
        printf("\t%s\n", arr[i]);
    }
}

//This function will parse the data_4.txt file
char** parseInt() {
    FILE *fp = fopen("data_4.txt", "r");

    char** array = malloc(sizeof(char*) * 30000);
    char line[200];

    //Loop 6000 times since the file has 6000 lines in it
    for (unsigned int i = 0; i < 6000; i++) {
        if (fgets(line, 200, fp) == NULL) {
            break;
        }

        //Parse the current line (do it 5 times since each line has 5 integers)
        int start = 0;
        int end = 0;
        for(int p = 0; p < 5; p++) {
            int j = 0;
            for(j = start; line[j] != ' '; j++);

            end = j;
            array[globalArrayLength] = malloc(sizeof(char) * (end - start) + 1);
            strncpy(array[globalArrayLength], line+start, end - start);
            array[globalArrayLength][end-start] = '\0';
            globalArrayLength++;

            start = end + 1;
        }
        
    }
    fclose(fp);
    return array;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please do ./assign3 [algorithm({1.1,1.2,2.1,2.2,2.3})]\n");
        return -1;
    }

    //Anagrams
    if (strcmp(argv[1],"1.1") == 0 || strcmp(argv[1],"1.2") == 0) {
        char** arr = parseInt();
        unsigned int numAnagrams = 0;
        char** Anagrams = NULL;

        //Run algorithm wanted.
        if (strcmp(argv[1],"1.1") == 0) {
            Anagrams = BruteForceAnagram(arr, globalArrayLength, &numAnagrams);
        } else {
            Anagrams = PreSortAnagram(arr, globalArrayLength, &numAnagrams);
        }

        printf("Anagrams: \n");
        printArray(Anagrams, numAnagrams);
        printf("Total Number of Anagrams: %d\n", numAnagrams);
    
        free2DArray(arr, globalArrayLength);
        free2DArray(Anagrams, numAnagrams);

    //String matching
    } else if (strcmp(argv[1], "2.1") == 0 || strcmp(argv[1],"2.2") == 0 || strcmp(argv[1],"2.3") == 0) {
        unsigned int matches = 0;
        unsigned int patternShifts = 0;

        //Run algorithm wanted.
        if (strcmp(argv[1], "2.1") == 0) {
            matches = bruteForceStringMatching(&patternShifts);
        } else if (strcmp(argv[1], "2.2") == 0) {
            matches = HorspoolStringMatching(&patternShifts);
        } else {
            printf("Not implemented yet.\n");
            matches = BoyerMooreStringMatching(&patternShifts);
        }

        printf("Matches: %d\n", matches);
        printf("Pattern Shifts: %d\n", patternShifts);

    } else {
        printf("Please enter one of the following next time: {1.1,1.2,2.1,2.2,2.3}\n");
        return -1;
    }

    return 0;
}