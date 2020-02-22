/**
 * Assignment 2. Part 1.1
 * Brute force algorithm to check for inversions 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Feb 10, 2020
 **/

#include <stdio.h>

//This will just check each pair of elements.
int brute_inversion(int* A, int length) {
    int i = 0, j = 0;
    int count = 0;
    for (i = 0; i < length - 1; i++) {
        for (j = i + 1; j < length; j++) {
            if (A[i] > A[j]) {
                count++;
            }
        }
    }

    return count;
}