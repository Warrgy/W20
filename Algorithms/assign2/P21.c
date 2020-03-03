/**
 * Assignment 2. Part 2.1
 * Brute force algorithm to check for end points in convex hull 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Feb 10, 2020
 **/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

int sizeofExtremeList = 0;

//Add the extreme point 'point' to the array of extreme points 'array', making sure it does not already exist.
void addPoint(Point* array, Point point) {
    for(int i = 0; i < sizeofExtremeList; i++) {
        if (array[i].x == point.x && array[i].y == point.y) {
            return;
        }
    }
    sizeofExtremeList++;
    array = (Point*)realloc(array, sizeof(Point) * sizeofExtremeList);
    array[sizeofExtremeList - 1] = point;
}

//Check every point with every points line and then check if every other point is on one side of the line. If so add it to the array Points
Point* brute_convex_hull(Point* list, int length, int* extremePNum) {
    Point* extremePoints = malloc(10000);

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i != j) {
                //Create formula with Point[i] and Point[j]
                //Formula is in representation of ax + by = 0
                //a = y2 - y1, b = x1 - x2 c = x1y2 - y1x2
                double a = ((list[j]).y) - ((list[i]).y);
                double b = ((list[i]).x) - ((list[j]).x);
                double c = (((list[i]).x) * ((list[j]).y)) - (((list[i]).y) * ((list[j]).x));

                int positive = 0;
                int negative = 0;
                for (int k = 0; k < length; k++) {
                    if (k != i && k != j) {
                        float result = 0;
                        //get result from point[k] with the line formula calculated already.
                        result = (a * ((list[k]).x)) + (b * ((list[k]).y)) - c;
                        //Checks to see where ax + by - c is > 0 or < 0.
                        if (result > 0) 
                            positive++;
                        else if (result < 0)
                            negative++;
                    }
                }
                //If positive or negative = 0, then the two points are extreme points.
                if (positive == 0 || negative == 0) {
                    //Add point[i] and point[j] into extreme points list
                    addPoint(extremePoints, list[i]);
                    addPoint(extremePoints, list[j]);
                }
            }
        }
    }
    *extremePNum = sizeofExtremeList;
    return extremePoints;
}