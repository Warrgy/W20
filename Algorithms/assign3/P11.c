/**
 * Assignment 3. Part 1.1
 * Brute Force for anagrams
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Mar 9, 2020
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char* getInput();

bool checkAnagram(char* indicator, char* toBeChecked) {
    if (strlen(indicator) != strlen(toBeChecked)) {
        return false;
    }
    int len = strlen(indicator);

    char* buffer = malloc(sizeof(char) * strlen(toBeChecked) + 1);
    strcpy(buffer, toBeChecked);

    for (int i = 0; i < len; i++) {
        for(int j = 0; j < len; j++) {
        }
    }
    free(buffer);
    return false;
}

char** BruteForceAnagram(char** array, unsigned int length, unsigned int* numAnagrams) {
    char* anagramString = getInput();

    for(int i = 0; i < length; i++) {
        if (checkAnagram(anagramString, array[i])) {
            //Add anagram to collection
            (*numAnagrams)++;
        }
    }

    free(anagramString);
    return NULL;
}