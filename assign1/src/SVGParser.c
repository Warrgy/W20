#include "../include/SVGParser.h"
#include "../include/LinkedListAPI.h"

SVGimage* createSVGimage(char* fileName) {
    return NULL;
}

char* SVGimageToString(SVGimage* img) {
    return NULL;
}

void deleteSVGimage(SVGimage* img) {
    
}

// Function that returns a list of all rectangles in the image.
List* getRects(SVGimage* img) {
    return NULL;
}

// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {
    return NULL;
}

// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {
    return NULL;
}

// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {
    return NULL;
}

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area) {
    return 0;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area) {
    return 0;
}

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data) {
    return 0;
}

int numAttr(SVGimage* img) {
    return 0;
}

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len) {
    return 0;
}

void deleteAttribute( void* data) {
    
}

char* attributeToString( void* data) {
    return NULL;
}

int compareAttributes(const void *first, const void *second) {
    return 0;
}

void deleteGroup(void* data) {
    
}

char* groupToString( void* data) {
    return NULL;
}

int compareGroups(const void *first, const void *second) {
    return 0;
}

void deleteRectangle(void* data) {
    
}

char* rectangleToString(void* data) {
    return NULL;
}

int compareRectangles(const void *first, const void *second) {
    return 0;
}

void deleteCircle(void* data) {
    
}

char* circleToString(void* data) {
    return NULL;
}

int compareCircles(const void *first, const void *second) {
    return 0;
}

void deletePath(void* data) {
    
}

char* pathToString(void* data) {
    return NULL;
}

int comparePaths(const void *first, const void *second) {
    return 0;
}

void print_elements(xmlNode *node) {
    
}

int main() {
    xmlDoc *doc = NULL;
    
    xmlNode *root_element = NULL;
    char *fileName = "quad01.svg";
    
    //LIBXML_TEST_VERSION
    
    doc = xmlReadFile(fileName, NULL, 0);
    
    if (doc == NULL) {
        printf("error: could not parse file %s.\n", fileName);
    }
    
//    root_element = xmlDocGetRootElement(doc);
    
    print_elements(root_element);
    
    
    
    printf("Makefile Test.\n");
    
    return 0;
}
