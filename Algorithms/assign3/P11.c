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

//Check if the character has already been found in the table. If it hasnt and still hasnt been found, then set that spot to a placeholder '+'.
void checkChar(char* table, char c, unsigned int tableLength) {
    for (int i = 0; i < tableLength; i++) {
        if (table[i] == c) {
            table[i] = '+';
            break;
        } 
    }
}

//Check each character in the string
void fillInTable(char* table, char* string) {
    for (int i = 0 ; i < strlen(string); i++) {
        checkChar(table, string[i], strlen(string));
    }
}

//Create table for string, then using that table determine if the two strings are anagrams.
bool checkAnagram(char* input, char* toBeChecked) {
    if (strlen(input) != strlen(toBeChecked)) {
        return false;
    }
    char* table = malloc(sizeof(bool) * strlen(input));
    
    for (int i = 0; i < strlen(input); i++) {
        table[i] = input[i];
    }

    fillInTable(table, toBeChecked);

    unsigned int count = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (table[i] == '+') {
            count++;
        }
    }
    free(table);

    if (count == strlen(input)) {
        return true;
    }

    return false;
}

//Find the total amount of anagrams
char** BruteForceAnagram(char** array, unsigned int length, unsigned int* numAnagrams) {
    char* anagramString = getInput();
    char** Anagrams = malloc(1);

    for(int i = 0; i < length; i++) {
        if (checkAnagram(anagramString, array[i])) {
            //Add anagram to collection
            (*numAnagrams)++;
            Anagrams = realloc(Anagrams, sizeof(char*) * (*numAnagrams));
            Anagrams[(*numAnagrams) - 1] = malloc(sizeof(char) * strlen(array[i]));
            strcpy(Anagrams[(*numAnagrams) - 1], array[i]);
        }
    }

    free(anagramString);
    return Anagrams;
}