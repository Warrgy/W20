/**
 * Assignment 3. Part 1.2
 * Presorting for anagrams
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* getInput();

int compare (const void * a, const void * b) {
    char x = *(int*) a;
    char y = *(int*) b;

   return ( x - y);
}

//Returns true is the two strings are anagrams, false otherwise.
bool checkPreSortAnagram(char* userInput, char* array) {
    if (strlen(userInput) != strlen(array)) {
        return false;
    }
    //Malloc and Sort the a duplicate string
    char* duplicate = malloc(sizeof(char) * strlen(array) + 1);
    strcpy(duplicate, array);
    qsort(duplicate, strlen(duplicate), sizeof(char), compare);

    if(strcmp(userInput, duplicate) == 0) {
        free(duplicate);
        return true;
    }

    free(duplicate);
    return false;
}

//Presort anagram algorithm
char** PreSortAnagram(char** array, unsigned int len, unsigned int* numAnagrams) {
    //Get user input and malloc an anagram 2D array
    char* input = getInput();
    char** Anagrams = malloc(sizeof(char*));
    
    //Sort the user input.
    qsort(input, strlen(input), sizeof(char), compare);

    for(int i = 0; i < len; i++) {
        if(checkPreSortAnagram(input, array[i])) {
            (*numAnagrams)++;
            Anagrams = realloc(Anagrams, sizeof(char*) * (*numAnagrams));
            Anagrams[(*numAnagrams) - 1] = malloc(sizeof(char) * strlen(array[i]));
            strcpy(Anagrams[(*numAnagrams) - 1], array[i]);
        }
    }

    free(input);
    return Anagrams;
}