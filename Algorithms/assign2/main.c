/**
 * Assignment 2. 
 * Main file to run algorithms
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Feb 10, 2020
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    double x;
    double y;
} Point;

int globalLengthArray = 0;

int brute_inversion(int*,int);
int divide_inversion(int*,int);
Point* brute_convex_hull(Point* list, int length, int* numExtremePoints);
Point* quick_hull(Point* list, int length, int* numExtremePoints);

//Gets the integers from the file and puts them into a dynamic array of ints.
int* getIntViaFile(char* name) {
    int i;
    char c = '0';

    FILE* fp = fopen(name,"r");
    if (fp == NULL) {
        printf("make sure %s is in the directory.\n",name);
        return NULL;
    }

    int* array = malloc(sizeof(int));
    char* integer = malloc(15);
    //Loop through file
    for (i = 1; ((c >= '0' && c <= '9') || c==' '|| c == '\n') ; i++) {
        int j = 0;
        //Get string representation of current integer
        for (; c != ' ' && c != '\n' && c != EOF && c != 0; j++) {
            integer[j] = c;
            c = getc(fp);
        }
        integer[j] = '\0';
        //Transer the string to an integer
        int test = atoi(integer);
        //Check for invalid values.
        if (test <= 0) {
            --i;
            c = getc(fp);
            continue;
        }
        //Add the integer to the array
        array= realloc(array, sizeof(int)*i);
        array[i - 1] = test;
        
        c = getc(fp);
    }
    globalLengthArray = i - 1;
    fclose(fp);
    free(integer);
    return array;
}

//Function to print an array of Points. ignore this
void printPoints(Point* points, int length) {
    printf("Points:\n");
    for(int i = 0 ; i < length; i++) {
        printf("(%lf,%lf)\n", points[i].x, points[i].y);
    }
}

//This will get all the points from a file and return an array of them.
Point* getPointsViaFile(char* fileName) {
    char c = '0';
    bool lever = false;
    bool gotFirstNum = false;
    
    //Used to add the chars into strings
    char* util = malloc(5);
    util[1] = '\0';

    Point* points = malloc(sizeof(Point));
    FILE* fp = fopen(fileName,"r");
    if (fp == NULL) {
        printf("make sure %s is in the directory.\n", fileName);
        return NULL;
    }

    //Go through file until all points have been seen
    for (int i = 0; !feof(fp) ;i++) {
        char* num1 = malloc(100); //Used for x
        char* num2 = malloc(100); //Used for y
        num1[0] = '\0';
        points = realloc(points, sizeof(Point) * (i + 1));

        //Get the current line of data. char by char.
        //Lever and gotFirstNum used to know when you got x and y
        //lever means transfer to y
        //gotFirstNum is whether you got the x yet. (for the spaces at the start)
        for(int j = 0; c != '\n'; j++) {
            c = getc(fp);
            if (c == EOF) {
                fclose(fp);
                free(num1);
                free(num2);
                free(util);
                return points;
            }
            //Got x already, flip lever
            if (c == ' ' && gotFirstNum) {
                lever = true;
            } 
            //Get x
            if (!lever) {
                if (c != ' ')
                    gotFirstNum = true;
                util[0] = c;
                strcat(num1,util);
            //Get y
            } else {
                util[0] = c;
                strcat(num2,util);
            }
        }
        //convert x and y to float and add them
        if (strlen(num1) > 0) {
            float x = atof(num1);
            if (x > 0.00001)
                points[i].x = x;
        }
        if (strlen(num2) > 0) {
            float y = atof(num2);
            if (y > 0.00001)
                points[i].y = y;
        }

        //Get reaady for next iteration.
        globalLengthArray++;
        free(num1);
        free(num2);
        c=getc(fp);
        lever = false;
        gotFirstNum = false;

    }
    //free and return points.
    free(util);
    fclose(fp);
    return points;
}

int main(int argc, char *argv[]) {
    clock_t start;
    int count = 0;

    if (argc != 2) {
        printf("Please do ./assign2 [algorithm]\n");
        return -1;
    }

    //Inversions
    if (strcmp(argv[1],"1.1") == 0 || strcmp(argv[1],"1.2") == 0) {
        int* array = getIntViaFile("data_1.txt");

        start = clock();
        //Run algorithm wanted.
        if (strcmp(argv[1],"1.1") == 0) {
            count = brute_inversion(array, globalLengthArray);
        } else {
            count = divide_inversion(array,globalLengthArray);
        }

        printf("Number of inversions = %d\n", count);
        free(array);
    //Convex Hull endpoints
    } else if (strcmp(argv[1], "2.1") == 0 || strcmp(argv[1],"2.2") == 0) {
        Point* points = getPointsViaFile("data_2.txt");
        int length = globalLengthArray;

        int numExtremePoints = 0;
        Point* p = NULL;
        start = clock();
        //Run algorithm wanted.
        if (strcmp(argv[1],"2.1") == 0) {
            p = brute_convex_hull(points, length, &numExtremePoints);
        } else if (strcmp(argv[1], "2.2") == 0) {
            p = quick_hull(points, length, &numExtremePoints);
        }
        printf("\nTotal number of extreme points = %d\n", numExtremePoints);
        printPoints(p,numExtremePoints);
        free(p);
        free(points);
    } else {
        printf("Please enter one of the following next time: {1.1,1.2,2.1,2.2}\n");
        return 0;
    }

    clock_t end = clock();
    double totalTime = (double)(end-start) / CLOCKS_PER_SEC;
    printf("\nAlgorithm time = %lfs\n", totalTime);
    return 0;
}