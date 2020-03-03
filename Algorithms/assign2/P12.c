/**
 * Assignment 2. Part 1.2
 * Divide and conquer algorithm to check for inversions 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Feb 10, 2020
 **/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

//This is just used for testing, ignore this.
//It just prints out the array.
void print_array(int* arr, int length) {
    printf("|");
    for (int i = 0; i < length; i++) {
        printf("%d | ", arr[i]);
    }
    printf("\tlength = %d\n", length);
    printf("\n");
}

//Will divide the array in half.
//@pre A has been pointed to the right part of the array needed to split.
//@var length -> length of the sub array needed
int* create_sub_array(int* A, int length) {
    int* arr = NULL;
    arr = malloc(sizeof(int) * length + 1);
    for (int i = 0; i < length; i++) {
        arr[i] = *A;
        ++A;
    }
    return arr;
}

//Will calculate the inversion on the endpoints for array B and C.
//Checks the last element in B with the first element in C.
int calculate_inversion(int* B, int* C, int* A, int Blen, int Clen) {
    int i = 0, j = 0, k = 0;
    int count = 0;

    while (i < Blen && j < Clen) {
        if (B[i] <= C[j]) {
            A[k] = B[i];
            i++;
        } else {
            count += (Blen - i);
            A[k] = C[j];
            j++;
        }
        k++;
    }
    if (i == Blen) {
        //Copy C[j..q-1] to A[k..p+q-1]
        for (int z = j; z < Clen; z++) {
            A[k] = C[z];
        }
    } else {
        //Copy B[i..p-1] to A[k..p+q-1]
        for (int z = i; z < Blen; z++) {
            A[k] = B[z];
        }
    }


    return count;
}

//Will recursively keep spliting up the array until one element, afterwards it will work its way back up and calculate inversion on the endpoints of the sub arrays
int divide_inversion(int* A, int length) {
    int count = 0;

    if (length > 1) {
        //Calculate the length that each sub arraay should be
        int Blen = (length / 2);
        int Clen = (length) - (length / 2);

        //Will point AforC to the right location in the array for C to split up.
        int* AforC = A + Blen;

        //Divide main array(array A) into two sub arrays B (lower half) and C(upper half).
        int* B  = create_sub_array(A, Blen);
        int* C = create_sub_array(AforC, Clen);

        //Recursively keep dividing
        count += divide_inversion(B, Blen);
        count += divide_inversion(C, Clen);

        //Calculate inversions in the two finished sub arrays B and C
        count += calculate_inversion(B, C, A, Blen, Clen);

        free(B);
        free(C);
    } 

    return count;
}