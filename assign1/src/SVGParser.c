/*
 * Assignment 1
 * CIS2750 
 *
 * Name: Cameron Fisher
 * ID: 1057347
 * Date: Jan 28, 2020
 */


#include "../include/SVGParser.h"
#include "../include/LinkedListAPI.h"
#include "assert.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

//Make sure the file name that is given, will be valid
char* checkFileName(char *file) {
    if (file == NULL)
        return NULL;
    
    if (strlen(file) == 0)
        return NULL;

    if(file[0] == 0 && file[strlen(file)] == 0) 
        return NULL;

    return file;
}

//Get the xmlDoc from the file, via xmllib2
xmlDoc* getDocViaFile(char *fileName) {
    xmlDoc* doc = NULL;

    LIBXML_TEST_VERSION
    doc = xmlReadFile(fileName, NULL, 0);

    /*if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
    }*/

    return doc;
}

//Checks to make sure the char can fit in a char[256] array
const unsigned char* checkLengthChar(const unsigned char *string) {
    if (string == NULL)
        return NULL;
    
    int length = strlen((char*)string);
    if (length > 255) {
        char* truncate = NULL;
        strncpy((char*)truncate, (char*) string, 255);
        return (const unsigned char*)truncate;
    }
    return string;
}

//Basically get the title string from the node
char* checkTitle(xmlNode* titleNode) {
    //TODO: also make sure if you find 1 title, dont do another one. Dont quote me on that. LOok it up.
    if (titleNode == NULL)
        return NULL;
    xmlNode* node = titleNode;
    const unsigned char* title = node->content;
    return (char*)(checkLengthChar(title));
}

//Get the description from the node
char* checkDescription(xmlNode* descNode) {
    if (descNode == NULL)
        return NULL;
    xmlNode *node = descNode;
    const unsigned char* desc = node->content; 
    return (char*)(checkLengthChar(desc));
}

//Basically get the namespace string from the node
const unsigned char* checkNameSpace(xmlNs* namespace) {
    if (namespace == NULL)
        return NULL;
    xmlNs* value = namespace;
    const unsigned char* URL = value->href;

    return checkLengthChar(URL);
}

//Will create and add an attribute to the list of attributes given
void addAttribute(const unsigned char* attr_name, const unsigned char* attr_value, List* listOfAttributes) {
    if (attr_name == NULL || attr_value == NULL)
        return;
    List* list = listOfAttributes;
    Attribute* curAttr = malloc(sizeof(Attribute));

    curAttr->name = malloc (strlen((const char*)attr_name) + 1);
    curAttr->value = malloc (strlen((const char*)attr_value) + 1);

    strcpy(curAttr->name, (char*)attr_name);
    strcpy(curAttr->value, (char*)attr_value);

    insertBack(list, (void*) curAttr);
}

//Will convert the string to a float, will also check for units
float stringToFloat(char* string, char units[50]) {
    if (string == NULL)
        return 0.0;
    int length = strlen(string);
    float value = 0.0;

    if ((string[length - 2] >= '0' && string[length - 2] <= '9') || string[length - 2] == '.') {
        //No units
        value = atof(string);
    } else {
        //Units
        units[0] = string[length - 2];
        units[1] = string[length - 1];
        units[2] = '\0';
        value = atof(string);
    }
    return value;
}

//Will create a rectangle with the information provided
void addRectangle(char* name, xmlNode* data, Rectangle* rectangle) {
    Rectangle* rect = rectangle;
    char* value = (char*)data->content;

    if (name == NULL || data == NULL)
        return;
    if (strcmp(name,"x") == 0) {
        rect->x = stringToFloat(value, rect->units);
    } else if (strcmp(name, "y") == 0) {
        rect->y = stringToFloat(value, rect->units);
    } else if (strcmp(name, "width") == 0) {
        rect->width = stringToFloat(value, rect->units);
    } else if (strcmp(name, "height") == 0) {
        rect->height = stringToFloat(value, rect->units);
    } else {
        //Attributes of the rectangle
        List* svgImageAttributes = rect->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a rectangle with the information in the node provided and add it to the list.
void addRectangleToList(xmlNode* node, List* rectList) {
    Rectangle *rect = malloc(sizeof(Rectangle));

    rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    rect->x = 0.0;
    rect->y = 0.0;
    strcpy(rect->units,"");

    for (xmlAttr *data = node->properties; data != NULL; data = data->next) {
        addRectangle((char*)data->name, data->children, rect);
    }
    insertBack(rectList, rect);
}

//Will create a circle with the information provided
void addCircle(char *name, xmlNode *data, Circle *circle) {
    Circle* circ = circle;
    char* value = (char*)data->content;

    if (strcmp(name, "cx") == 0) {
        circ->cx = stringToFloat(value, circ->units);
    } else if (strcmp(name, "cy") == 0) {
        circ->cy = stringToFloat(value, circ->units);
    } else if (strcmp(name, "r") == 0) {
        circ->r = stringToFloat(value, circ->units);
    } else {
        List* svgImageAttributes = circ->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a circle with the information in the node provided and add it to the list.
void addCircleToList(xmlNode* node, List* circList) {
    Circle *circle = malloc(sizeof(Circle));

    circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    circle->cy = 0.0;
    circle->cx = 0.0;
    strcpy(circle->units, "");

    for (xmlAttr *data = node->properties; data != NULL; data = data->next) {
        addCircle((char*)data->name, data->children, circle);
    }
    insertBack(circList, circle);
}

//Will create a path with the information provided
void addPath(char *name, xmlNode* data, Path* thePath) {
    Path* path = thePath;
    char* value = (char*)data->content;

    if (strcmp(name, "d") == 0) {
        path->data = malloc(strlen((char*)value) + 1);
        strcpy(path->data, value);
    } else {
        List* svgImageAttributes = path->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a path with the information in the node provided and add it to the list.
void addPathToList(xmlNode* node, List* pathList) {
    Path *path = malloc(sizeof(Path));

    path->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    //TODO: setting data to nothing if doesnt have datastrcpy(path->data, "");

    for (xmlAttr* data = node->properties; data != NULL; data = data->next) {
        addPath((char*)data->name, data->children, path);
    }
    insertBack(pathList, path);
}

//This will go through the xml tree recursively and add the required information to the SVGimage struct.
SVGimage* convertXMLtoSVG(xmlNode* a_node, SVGimage* image) {
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
            printf("Current Loop name: [%s]\n", cur_node->name);

            //If cur_node->nsDef != NULL -> deal with namespace
            //Add and check the NAMESPACE to the SVGimage ----
            if (checkNameSpace(cur_node->nsDef) != NULL) {
                const unsigned char* ns = checkNameSpace(cur_node->nsDef);
                strcpy(image->namespace, (char*) ns);
            }
            //-------------
        
            xmlNode *paren = cur_node->parent;

            //if name = title
            //Add and check the TITLE to the SVGimage -------
            if (paren->name !=NULL) {
                if (strcmp((char*)paren->name, (char*)"title") == 0) {
                    if (checkTitle(cur_node) != NULL) {
                        strcpy(image->title, checkTitle(cur_node));
                        printf("Got the title. it is [%s]\n", image->title);
                    }
                }
                //---------------------


                //if name = desc
                //Add and check the DESCRIPTION to the SVGimage ------
                else if (strcmp((char*)paren->name, (char*)"desc") == 0) {
                    if (checkDescription(cur_node) != NULL) {
                        strcpy(image->description, checkDescription(cur_node));
                        printf("Got the description. it is [%s]\n", image->description);
                    }
                }
            //--------------
            }

            //if parent name = svg
            //Add all the attributes of the svg into the otherAtrributes list in the SVGimage.
            if (strcmp((char*)cur_node->name,(char*)"svg") == 0) {
                xmlAttr *attr_node = NULL;
                List* svgImageAttributes = image->otherAttributes;

                //Loop through all the attributes
                for (attr_node = cur_node->properties; attr_node != NULL; attr_node = attr_node->next) {
                    xmlNode* alsoTemp = attr_node->children;                  
                    addAttribute(attr_node->name, alsoTemp->content, svgImageAttributes);
                }
            }

            //if name = rect, while not in g
            if (strcmp((char*)(cur_node->name),(char*)"rect") == 0) {
                addRectangleToList(cur_node, image->rectangles);
            }

            //if name = circle, not in g
            if (strcmp((char*)(cur_node->name), (char*)"circle") == 0) {
                if (strcmp((char*)(paren->name), (char*)"g") != 0) {
                    //circle, parent is not a g.
                    addCircleToList(cur_node, image->circles);
                }
                //circle, but parent is a g
                printf("parent is a g\n");
            }

            //if name = path
            if (strcmp((char*)(cur_node->name), (char*)"path") == 0) {
                if (strcmp((char*)(paren->name), (char*)"g") != 0) {
                    //path, but parent is not a g
                    addPathToList(cur_node, image->paths);
                    printf("found path.\n");
                }
                //path, but parent is a g
            }

            //Deal with g if not already dealt with
        
        if (cur_node->children != NULL) {
            printf("Going into recursion.\n");
            convertXMLtoSVG(cur_node->children,image);
        }   
        if (cur_node->next == NULL)
            break;
    }
    return image;
}

//This will initialize all the elements in the struct, and then fill the struct with all the data.
SVGimage* initializeSVG(xmlNode *a_node) {
    SVGimage *image = malloc(sizeof(SVGimage));
    strcpy(image->description,"");
    strcpy(image->title,"");
    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);

    SVGimage *fullSVG = convertXMLtoSVG(a_node, image); 

    return fullSVG;
}

//This will open a file and create a SVGimaage struct using the XML data in that file.
SVGimage* createSVGimage(char* fileName) {
    //Check file name
    char *name = checkFileName(fileName);
    if (name == NULL) 
        return NULL;
    //Use xmllib2 to get tree. Then put into xmlDoc & check
    xmlDoc *doc = getDocViaFile(fileName);
    if (doc == NULL)
        return NULL;
    //xmlDoc -> xmlNode
    xmlNode *root = xmlDocGetRootElement(doc);
    if (root == NULL)
        return NULL;

    //print_element_names(root);
    SVGimage* image = initializeSVG(root);
    if (image == NULL)
        return NULL;

    char* details = SVGimageToString(image);
    printf("\nDetails:\n%s\n", details);
    free(details);
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    printf("Done.\n");
    return image;
}

char* SVGimageToString(SVGimage* img) {
    char *string = NULL;
    char *svgAttr = toString(img->otherAttributes);
    char *rect_list = toString(img->rectangles);
    char *circle_list = toString(img->circles);
    char *path_list = toString(img->paths);

    string = malloc(  strlen((char*)(img->namespace)) + 13 
                    + strlen((char*)(img->title)) + 9 
                    + strlen((char*)(img->description)) + 15
                    + strlen(svgAttr) + 11 
                    + strlen(rect_list) + 1
                    + strlen(circle_list) + 1
                    + strlen(path_list) + 1
                    + 1);

    sprintf(string, "Namespace = %s\nsvg attr: %s\nTitle = %s\nDescription = %s\n%s\n%s\n%s\n",
                     img->namespace, svgAttr, img->title, img->description, rect_list, circle_list, path_list); 

    free(svgAttr);
    free(rect_list);
    free(circle_list);
    free(path_list);
    return string;
}

void deleteSVGimage(SVGimage* img) {
    //free paths
    clearList(img->paths);
    freeList(img->paths);

    //free circles
    clearList(img->circles);
    freeList(img->circles);

    //free rectangles
    clearList(img->rectangles);
    freeList(img->rectangles);

    //free svg attributes
    clearList(img->otherAttributes);
    freeList(img->otherAttributes);

    //free SVGimage
    free(img);
}

// Function that returns a list of all rectangles in the image.
List* getRects(SVGimage* img) {
    return img->rectangles;
}

// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {
    return img->circles;
}

// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {
    return img->groups;
}

// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {
    return img->paths;
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

//How to free an Attribute struct
void deleteAttribute( void* data) {
    Attribute* attr = (Attribute*) data;
    free(attr->name);
    free(attr->value);
    free(attr);
}

//How to represent the data in an Attribute struct in terms of a string
char* attributeToString( void* data) {
    Attribute* attr = (Attribute*) data;
    char* string = malloc(  strlen(attr->name) + 3 +
                            strlen(attr->value) + 3 +
                            + 1 );

    sprintf(string, "   %s = %s", attr->name, attr->value);

    return string;
}

//How to compare Attributes
int compareAttributes(const void *first, const void *second) {
    const Attribute* one = (const Attribute*) first;
    const Attribute* two = (const Attribute*) second;

    if (strcmp(one->name, two->name) == 0) {
        if (strcmp(one->value, two->value) == 0) {
            return 0;
        } else
            return strcmp(one->value, two->value);
    } else 
        return strcmp(one->name, two->name);
    return -1;
}

void deleteGroup(void* data) {
    
}

char* groupToString( void* data) {
    return NULL;
}

int compareGroups(const void *first, const void *second) {
    return 0;
}

//How to free a rectangle struct
void deleteRectangle(void* data) {
    Rectangle* rect = (Rectangle*) data;

    clearList(rect->otherAttributes);
    freeList(rect->otherAttributes);

    free(rect);
}

//How to represent the data in a Rectangle struct in terms of a string
char* rectangleToString(void* data) {
    Rectangle* rect = (Rectangle*) data;
    List* rect_attr = rect->otherAttributes;
    char* rect_attr_string = toString(rect_attr);

    char* string = malloc(  (9 * 4 * 15) + 11 + 7 + 7 + 11 + 12 + (11 + 2) + strlen(rect_attr_string) + 1 + 1);

    sprintf(string, "Rectangle:\n  x = %.4f\n  y = %.4f\n  width = %.4f\n  height = %.4f\n  units = %s\n%s\n",
                rect->x, rect->y, rect->width, rect->height, rect->units, rect_attr_string);

    free(rect_attr_string);
    return string;
}

//How to compare Rectangles
int compareRectangles(const void *first, const void *second) {
    //TODO: add compare for the units also
    const Rectangle* one = (const Rectangle* ) first;
    const Rectangle* two = (const Rectangle* ) second;

    if (one->x == two->x)
        if (one->y == two->y)
            if (one->width == two->width)
                if (one->height == two->height)
                    return 0;
    return -1;
}

//How to free a circle struct
void deleteCircle(void* data) {
    Circle *c = (Circle*) data;

    clearList(c->otherAttributes);
    freeList(c->otherAttributes);

    free(c);
}

//How to represent the data in a Circle struct in terms of a string
char* circleToString(void* data) {
    Circle *circle = (Circle*) data;
    List* circle_attr = circle->otherAttributes;
    char* circle_attr_string = toString(circle_attr);

    char *string = malloc((9 * 3 * 15) + 8 + 8 + 8 + 7 + (11 + strlen(circle->units)) + (strlen(circle_attr_string) + 1) + 1);

    sprintf(string, "Circle:\n  cx = %.4f\n  cy = %.4f\n  r = %.4f\n  units = %s\n%s\n",
                circle->cx, circle->cy, circle->r, circle->units, circle_attr_string);

    free(circle_attr_string);
    return string;
}

//How to compare Circles
int compareCircles(const void *first, const void *second) {
    const Circle *one = (const Circle*) first;
    const Circle *two = (const Circle*) second;

    if (one->cx == two->cx)
        if (one->cy == two->cy)
            if (one->r == two->r)
                return 0;
    return -1;
}

//How to free a path struct
void deletePath(void* data) {
    Path *path = (Path*) data;

    free(path->data);
    clearList(path->otherAttributes);
    freeList(path->otherAttributes);

    free(path);
}

//How to represent the data in a Path struct in terms of a string
char* pathToString(void* data) {
    Path *path = (Path*) data;
    List* path_attr = path->otherAttributes;
    char* path_attr_string = toString(path_attr);

    char *string = malloc(6 + 7 + strlen(path->data) + strlen(path_attr_string) + 1 + 1);

    sprintf(string, "Path:\n  d = %s\n%s\n",
                path->data, path_attr_string);

    free(path_attr_string);
    return string;
}

//How to compare Paths
int comparePaths(const void *first, const void *second) {
    const Path* one = (const Path*) first;
    const Path* two = (const Path*) second;

    return strcmp(one->data, two->data) == 0;
}

int main() {
    char* file = malloc(20);
    //strcpy(file, "quad01.svg");
    strcpy(file, "Emoji_poo.svg");
    //strcpy(file, "rect.svg");
    //strcpy(file, "circle.svg");
    SVGimage* img = createSVGimage(file);

    deleteSVGimage(img);

    printf("Makefile Test.\n");
    
    free(file);
    return 0;
}
