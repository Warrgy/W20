/**
 * Assignment 2. Part 2.2
 * Quickhull Algorithm to find the end points in a convex hull. 
 *
 * Name = Cameron Fisher
 * ID = 1057347
 * Date = Feb 10, 2020
 **/

#include <stdio.h>
#include <stdlib.h>

int globalEndpointLength = 0;

typedef struct {
    double x;
    double y;
} Point;

//Will recursively find all endpoints in the array of Points. Then add them all to 'extremePoints'.
//Will create line from A and B, then find max point on the correct side of the line. Then do it with those lines.. so in
void sub_hull(Point* A, Point* B, Point* points, int size, Point* extremePoints, char side) {
    double max = 0;
    Point* maxPoint = NULL;
    int s1size = 0;
    int s2size = 0;

    Point* s1 = malloc(10000);
    Point* s2 = malloc(10000);
    Point* startS1 = s1;
    Point* startS2 = s2;

    //Make line from A to B
    //Formula is in representation of ax + by = 0
    //a = y2 - y1, b = x1 - x2 c = x1y2 - y1x2
    double a = B->y - A->y;
    double b = A->x - B->x;
    double c = (A->x * B->y) - (B->x * A->y);

    for (int i = 0; i < size; i++) {
        //Subin point using ax + by - c = 0
        double result = (a * points[i].x) + (b * points[i].y) - c;
        // Make sure point is not (0.0,0.0)
        if ((points[i].x < -0.0001 || points[i].x > 0.0001) && (points[i].y < -0.0001 || points[i].y > 0.0001)) {
            //Make the subset of all above and below the line
            if (side == 'u') {
                //Add point to s1 if it is > 0
                if (result > .00001) {
                    s1size++;
                    s1 = realloc(s1, sizeof(Point) * s1size);
                    s1[s1size - 1] = points[i];
                }
                //Determine max
                if (result > max) {
                    maxPoint = &(points[i]); 
                    max = result;
                }
            } else {
                //Add point to s2 if it is < 0
                if (result < -0.0001) {
                    s2size++;
                    s2 = realloc(s2, sizeof(Point) * s2size);
                    s2[s2size - 1] = points[i];
                } 
                if (result < max) {
                    maxPoint = &(points[i]); 
                    max = result;
                }
            }
        }
    }

    //Add maxPoint to the end points(extremePoints) and then call function again using this max number.
    if (maxPoint != NULL) {
        if ((maxPoint->x != A->x && maxPoint->y != A->y) && (maxPoint->x != B->x && maxPoint->y != B->y)) {
            globalEndpointLength++;
            extremePoints[globalEndpointLength - 1] = *maxPoint;
            if (side == 'u') {
                sub_hull(A, maxPoint, startS1, s1size, extremePoints, 'u');
                sub_hull(B, maxPoint, startS2, s2size, extremePoints, 'l');
            } else if (side == 'l') {
                sub_hull(A, maxPoint, startS1, s1size, extremePoints, 'l');
                sub_hull(B, maxPoint, startS2, s2size, extremePoints, 'u');
            }
        }
    }
}

//Go through array of points and find the min and max pointed based off of their x and pass them into the sub_hull function to find other endpoints.
Point* quick_hull(Point* P, int length, int* numExtremePoints) {
    if (length < 1) {
        return NULL;
    }

    Point* endpoints = NULL;
    float min = P[0].x; 
    float max = P[0].x; 
    Point* maxP = &(P[0]); 
    Point* minP = &(P[0]);

    //Find the max and min point
    for (int i = 0; i < length; i++) {
        if (P[i].x < min) {
            min = P[i].x;
            minP = &(P[i]);
        }
        if (P[i].x > max) {
            max = P[i].x;
            maxP = &(P[i]);
        }
    }
    //Add endpoints to list. If only got one point (i.e max == min) then return the points.
    if (max != min) {
        endpoints = malloc(sizeof(Point) * 2);
        endpoints[0] = *minP;
        endpoints[1] = *maxP;
        globalEndpointLength += 2;
    } else {
        globalEndpointLength = 1;
        *numExtremePoints = globalEndpointLength;
        return maxP;
    }
    
    //Get all other endpoints
    sub_hull(minP, maxP, P, length, endpoints, 'u');
    sub_hull(minP, maxP, P, length, endpoints, 'l');

    *numExtremePoints = globalEndpointLength;
    return endpoints;
}