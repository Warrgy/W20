/*
 * SVG Assignment
 * CIS2750 
 *
 * Name: Cameron Fisher
 * Email: cfishe08@uoguelph.ca
 * ID: 1057347
 * Date: Jan 28, 2020
 *
 * Portions of this code were based off LinkedListAPI.c
 * Used http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html as reference for validating xml files. (Was include in the assignment description)
 */

#include <math.h>
#include <ctype.h>
#include "SVGParser.h"
#include "LinkedListAPI.h"

#define PI 3.1415926535

//Make sure the file name that is given, will be valid
static char* checkFileName(char *file) {
    if (file == NULL)
        return NULL;
    int length = strlen(file);
    if (length == 0)
        return NULL;

    if (strcmp(file, "") == 0)
        return NULL;

    if(file[0] == 0 && file[length] == 0) 
        return NULL;

    return file;
}

//Get the xmlDoc from the file, via xmllib2
static xmlDoc* getDocViaFile(char *fileName) {
    xmlDoc* doc = NULL;

    LIBXML_TEST_VERSION
    doc = xmlReadFile(fileName, NULL, 0);

    /*if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
    }*/

    return doc;
}

//Checks to make sure the char can fit in a char[256] array. Returns dynamic string
static const unsigned char* checkLengthChar(const unsigned char *string) {
    if (string == NULL)
        return NULL;
    
    int length = strlen((char*)string);

    if (length > 255) {
        char* truncate = malloc(256);
        strncpy((char*)truncate, (char*) string, 255);
        return (const unsigned char*)truncate;
    } else {
        char* str = malloc(strlen((char*)string) + 1);
        strcpy((char*)str, (char*)string);
        return (const unsigned char*)str;
    }
}

//Check the title string from the node and return
static char* checkTitle(xmlNode* titleNode) {
    if (titleNode == NULL)
        return NULL;
    xmlNode* node = titleNode;
    const unsigned char* title = node->content;
    return (char*)(checkLengthChar(title));
}

//Get the description from the node
static char* checkDescription(xmlNode* descNode) {
    if (descNode == NULL)
        return NULL;
    xmlNode *node = descNode;
    const unsigned char* desc = node->content; 
    return (char*)(checkLengthChar(desc));
}

//Basically get the namespace string from the node
static const unsigned char* checkNameSpace(xmlNs* namespace) {
    if (namespace == NULL)
        return NULL;
    xmlNs* value = namespace;
    const unsigned char* URL = value->href;

    return checkLengthChar(URL);
}

//Will make the string entirely lower case.
static char* checkUppLowCase(char* string) {
    /*int length = strlen(string);
    for (int i = 0; i < length; i++) {
        string[i] = tolower(string[i]);
    }*/
    return string;
}

//Will create and add an attribute to the list of attributes given
static void addAttribute(const unsigned char* attr_name, const unsigned char* attr_value, List* listOfAttributes) {
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
static float stringToFloat(char* string, char units[50]) {
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
static void addRectangle(char* name, xmlNode* data, Rectangle* rectangle) {
    Rectangle* rect = rectangle;
    char* value = (char*)data->content;

    if (name == NULL || data == NULL)
        return;
    if (strcmp(checkUppLowCase(name),"x") == 0) {
        rect->x = stringToFloat(value, rect->units);
    } else if (strcmp(checkUppLowCase(name), "y") == 0) {
        rect->y = stringToFloat(value, rect->units);
    } else if (strcmp(checkUppLowCase(name), "width") == 0) {
        rect->width = stringToFloat(value, rect->units);
    } else if (strcmp(checkUppLowCase(name), "height") == 0) {
        rect->height = stringToFloat(value, rect->units);
    } else {
        //Attributes of the rectangle
        List* svgImageAttributes = rect->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a rectangle with the information in the node provided and add it to the list.
static void addRectangleToList(xmlNode* node, List* rectList) {
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
static void addCircle(char *name, xmlNode *data, Circle *circle) {
    Circle* circ = circle;
    char* value = (char*)data->content;

    if (strcmp(checkUppLowCase(name), "cx") == 0) {
        circ->cx = stringToFloat(value, circ->units);
    } else if (strcmp(checkUppLowCase(name), "cy") == 0) {
        circ->cy = stringToFloat(value, circ->units);
    } else if (strcmp(checkUppLowCase(name), "r") == 0) {
        circ->r = stringToFloat(value, circ->units);
    } else {
        List* svgImageAttributes = circ->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a circle with the information in the node provided and add it to the list.
static void addCircleToList(xmlNode* node, List* circList) {
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
static void addPath(char *name, xmlNode* data, Path* thePath) {
    Path* path = thePath;
    char* value = (char*)data->content;

    if (strcmp(checkUppLowCase(name), "d") == 0) {
        path->data = malloc(strlen((char*)value) + 1);
        strcpy(path->data, value);
    } else {
        List* svgImageAttributes = path->otherAttributes;
        
        addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    }
}

//Will create a path with the information in the node provided and add it to the list.
static void addPathToList(xmlNode* node, List* pathList) {
    Path *path = malloc(sizeof(Path));

    path->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    //TODO: setting data to nothing if doesnt have datastrcpy(path->data, "");

    for (xmlAttr* data = node->properties; data != NULL; data = data->next) {
        addPath((char*)data->name, data->children, path);
    }
    insertBack(pathList, path);
}

//Will create a group with the information provided (mean't for attributes)
static void addGroupAttributes(char* name, xmlNode* data, Group* theGroup) {
    Group* group = theGroup;
    List* svgImageAttributes = group->otherAttributes;
        
    addAttribute((const unsigned char*)name, data->content, svgImageAttributes);
    
}

//Will create a group with the information in the node provided and add it to the list. If a group in a group happens, then we use recursion.
static void addGroupToList(xmlNode *node, List* groupList) {
    Group *group = malloc(sizeof(Group));

    //Initialize all lists before using
    group->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    group->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    group->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    group->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    group->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    //Add the attributes for the list here
    for (xmlAttr* data = node->properties; data != NULL; data = data->next) {
        addGroupAttributes((char*)data->name, data->children, group);        
    }

    //Add rectangles, circles, paths, and groups to each respective list
    for (xmlNode* child = node->children; child != NULL; child = child->next) {
        if (strcmp(checkUppLowCase((char*)(child->name)), "rect") == 0) {
            addRectangleToList(child, group->rectangles);
        } else if (strcmp(checkUppLowCase((char*)(child->name)), "circle") == 0) {
            addCircleToList(child, group->circles);
        } else if (strcmp(checkUppLowCase((char*)(child->name)), "path") == 0) {
            addPathToList(child, group->paths);
        } else if (strcmp(checkUppLowCase((char*)(child->name)), "g") == 0) {
            addGroupToList(child, group->groups);
        } else {
            //left blank for now
            //This is if we add more implementation for other types
        }
    }
    insertBack(groupList, group);
}

//This will go through the xml tree recursively and add the required information to the SVGimage struct.
static SVGimage* convertXMLtoSVG(xmlNode* a_node, SVGimage* image) {
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
            //printf("Current Loop name: [%s]\n", cur_node->name);

            //If cur_node->nsDef != NULL -> deal with namespace
            //Add and check the NAMESPACE to the SVGimage ----
            if (cur_node->nsDef != NULL) {
                unsigned char* ns = (unsigned char*)checkNameSpace(cur_node->nsDef);
                if (ns != NULL) {
                    strcpy(image->namespace, (char*) ns);
                    free(ns);
                }
            }
            //-------------
        
            xmlNode *paren = cur_node->parent;

            //if name = title
            //Add and check the TITLE to the SVGimage -------
            if (paren->name !=NULL) {
                if (strcmp(checkUppLowCase((char*)paren->name), (char*)"title") == 0) {
                    char* temp = checkTitle(cur_node);
                    if (temp != NULL) {
                        strcpy(image->title, temp);
                        free(temp);
                    }
                }
                //---------------------


                //if name = desc
                //Add and check the DESCRIPTION to the SVGimage ------
                else if (strcmp(checkUppLowCase((char*)paren->name), (char*)"desc") == 0) {
                    char* temporary = checkDescription(cur_node);
                    if (temporary != NULL) {
                        strcpy(image->description, temporary);
                        free(temporary);
                    }
                }
            //--------------
            }

            //if parent name = svg
            //Add all the attributes of the svg into the otherAtrributes list in the SVGimage.
            if (strcmp(checkUppLowCase((char*)cur_node->name),(char*)"svg") == 0) {
                xmlAttr *attr_node = NULL;
                List* svgImageAttributes = image->otherAttributes;

                //Loop through all the attributes
                for (attr_node = cur_node->properties; attr_node != NULL; attr_node = attr_node->next) {
                    xmlNode* alsoTemp = attr_node->children;                  
                    addAttribute(attr_node->name, alsoTemp->content, svgImageAttributes);
                }
            }

            //if name = rect, while not in g
            if (strcmp(checkUppLowCase((char*)(cur_node->name)),(char*)"rect") == 0) {
                if (strcmp(checkUppLowCase((char*) (paren->name)), (char*)"g") != 0) {
                    addRectangleToList(cur_node, image->rectangles);
                }
            }

            //if name = circle, not in g
            if (strcmp(checkUppLowCase((char*)(cur_node->name)), (char*)"circle") == 0) {
                if (strcmp(checkUppLowCase((char*)(paren->name)), (char*)"g") != 0) {
                    //circle, parent is not a g.
                    addCircleToList(cur_node, image->circles);
                }
                //circle, but parent is a g
            }

            //if name = path, not in g
            if (strcmp(checkUppLowCase((char*)(cur_node->name)), (char*)"path") == 0) {
                if (strcmp(checkUppLowCase((char*)(paren->name)), (char*)"g") != 0) {
                    //path, but parent is not a g
                    addPathToList(cur_node, image->paths);
                }
                //path, but parent is a g
            }
            
            //If a group then add to group list
            if (strcmp(checkUppLowCase((char*)(cur_node->name)), (char*)"g") == 0) {
                if (strcmp(checkUppLowCase((char*)(paren->name)), (char*)"g") != 0) {
                    addGroupToList(cur_node, image->groups);
                }
            }
        
        //If this node has children then go to them
        if (cur_node->children != NULL) {
            convertXMLtoSVG(cur_node->children,image);
        }   
        //if there is no node that is next in the link then break the loop.
        if (cur_node->next == NULL)
            break;
    }
    return image;
}

//This will initialize all the elements in the struct, and then fill the struct with all the data.
//Will initialize all lists and set the title and description to blank strings.
static SVGimage* initializeSVG(xmlNode *a_node) {
    SVGimage *image = malloc(sizeof(SVGimage));
    strcpy(image->description,"");
    strcpy(image->title,"");
    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

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
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return image;
}

//Will create a string to display the entire contents of the SVGimage struct
char* SVGimageToString(SVGimage* img) {
    if (img == NULL)
        return NULL;
    char *string = NULL;
    char *svgAttr = toString(img->otherAttributes);
    char *rect_list = toString(img->rectangles);
    char *circle_list = toString(img->circles);
    char *path_list = toString(img->paths);
    char *group_list = toString(img->groups);

    string = malloc(  strlen((char*)(img->namespace)) + 13 
                    + strlen((char*)(img->title)) + 9 
                    + strlen((char*)(img->description)) + 15
                    + strlen(svgAttr) + 11 
                    + strlen(rect_list) + 1
                    + strlen(circle_list) + 1
                    + strlen(path_list) + 1
                    + strlen(group_list) + 1
                    + 1);

    sprintf(string, "Namespace = %s\nsvg attr: %s\nTitle = %s\nDescription = %s\n%s\n%s\n%s\n%s\n",
                     img->namespace, svgAttr, img->title, img->description, rect_list, circle_list, path_list, group_list); 

    free(svgAttr);
    free(rect_list);
    free(circle_list);
    free(path_list);
    free(group_list);
    return string;
}

//Will free the entire SVGimage
void deleteSVGimage(SVGimage* img) {
    if (img == NULL)
        return;
    //free groups
    clearList(img->groups);
    freeList(img->groups);

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

//Prints a list
/*static void printList(List* type) {
    if (type == NULL)
        return;
    char* string = toString(type);

    printf("string = %s\n", string);

    free(string);
}*/

//Traverse through the list and add all nodes to the new list.
static void addListElemToDiffList(List* new, List* old) {
    if (new == NULL || old == NULL)
        return;
    List* copy = old;
    ListIterator itr = createIterator(copy);
    void* node = nextElement(&itr);
    while (node != NULL)
	{
        insertBack(new, node);
        
		node = nextElement(&itr);
	}
}

//Recursively add all rectangles, circles, paths, or groups in the list. If there are, call addListElemToDiffList to actually add them.
static void addGListsToList(List* new, List* old, char type) {
    if (old == NULL)
        return;
    List* groups = old;

    ListIterator itr = createIterator(groups);
    void* node = nextElement(&itr);

    while (node != NULL)
	{
        Group* group = (Group*) node;
        if (type == 'r') {
            addListElemToDiffList(new, group->rectangles);
        } else if (type == 'c') {
            addListElemToDiffList(new, group->circles);
        } else if (type == 'p') {
            addListElemToDiffList(new, group->paths);
        } else if (type == 'g') {
            addListElemToDiffList(new, group->groups);
        }
        addGListsToList(new, group->groups, type);
		node = nextElement(&itr);
	}
}

//For freeing
void dummyFunction() {

}

// Function that returns a list of all rectangles in the image.
List* getRects(SVGimage* img) {
    if (img == NULL)
        return NULL;
    List* rects = initializeList(&rectangleToString, &dummyFunction, &compareRectangles);
    
    //Get rectangles in the immediate image
    addListElemToDiffList(rects, img->rectangles);
    //Add rectangles located in the groups
    addGListsToList(rects, img->groups, 'r');

    return rects;
}

// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {
    if (img == NULL)
        return NULL;
    List* circ = initializeList(&circleToString, &dummyFunction, &compareCircles);

    //Get circles in the immediate image
    addListElemToDiffList(circ, img->circles);
    //Add circles located in the groups
    addGListsToList(circ, img->groups, 'c');

    return circ;
}

// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {
    if (img == NULL)
        return NULL;
    List* group = initializeList(&groupToString, &dummyFunction, &compareGroups);

    //Get groups in the immediate image
    addListElemToDiffList(group, img->groups);
    //Add groups located in the groups
    addGListsToList(group, img->groups, 'g');
    
    return group;
}

// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {
    if (img == NULL)
        return NULL;
    List* paths = initializeList(&pathToString, &dummyFunction, &comparePaths);

    //Get paths in the immediate image
    addListElemToDiffList(paths, img->paths);
    //Add paths located in the groups
    addGListsToList(paths, img->groups, 'p');

    return paths;
}

/**This function will return the number of times either rectangle, circle or path is equal to its corresponding set of data.
 *This function is very generic and will only work if you are trying to find:
 *
 * Rectangle -> Area
 * Circle -> Area
 * Path -> data (string)
 *
 * 
**/
static int numGWithArea(List* list, int area, char* pathData, char type, const char wanted) {
    if (list == NULL)
        return 0;
    int count = 0;
    ListIterator itr = createIterator(list);
    void* data = nextElement(&itr);


	while (data != NULL)
	{
        //Current node = rectangle and that is wanted. Find out if it equals area
        if (type == 'r' && wanted == 'r') {
            if (area < 0)  //Shouldn't be needed, but mind as well check
                return 0;
            Rectangle* current = (Rectangle*) data;
            int product = ceil(current->height * current->width);
            if (product == area) {
                count++;
            }
        //Current node = circle and that is wanted. Find out if it equals area
        } else if (type == 'c' && wanted == 'c') {
            if (area < 0)  //Shouldn't be needed, but mind as well check
                return 0;
            Circle* current = (Circle*) data;
            int total = ceil(current->r * current->r * PI);
            if (total == area) {
                count++;
            }
        //Current node = path and that is wanted. Find out if it equals the data
        } else if (type == 'p' && wanted == 'p') {
            if (pathData == NULL)  //Shouldn't be needed, but mind as well check
                return 0;
            Path* path = (Path*) data;
            if (strcmp(pathData, path->data) == 0) {
                count++;
            }
        //Check all the lists in the groups and check rectangles or circles
        } else if (type == 'g') {
            Group* g = (Group*) data;
            count += numGWithArea(g->rectangles, area, NULL, 'r', wanted);
            count += numGWithArea(g->circles, area, NULL, 'c', wanted);
            count += numGWithArea(g->paths, -1, pathData, 'p', wanted);
            count += numGWithArea(g->groups, area, pathData, 'g', wanted);
        }
        
		data = nextElement(&itr);
	}
    return count;
}

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area) {
    if (img == NULL)
        return 0;
    if (area < 0)
        return 0;

    int count = 0;
    ListIterator itr = createIterator(img->rectangles);
    void* data = nextElement(&itr);

    Rectangle* current = NULL;
    int product = 0;

	while (data != NULL)
	{
        //Check if the rectangle has the area specified
        current = (Rectangle*) data;
        product = ceil(current->height * current->width);
        if (product == ceil(area)) {
            count++;
        }
        
		data = nextElement(&itr);
	}
    
    //Count all the rectangles with the area inside the groups.
    count += numGWithArea(img->groups, ceil(area), NULL, 'g', 'r');

    return count;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area) {
    if (img == NULL)
        return 0;
    if (area < 0)
        return 0;

    int count = 0;
    ListIterator itr = createIterator(img->circles);
    void* data = nextElement(&itr);

    Circle* current = (Circle*) data;
    int product = 0;

    while (data != NULL)
	{
        //Check if the circle has the area specifiec
        current = (Circle*) data;
        product = ceil(current->r * current->r * PI);
        if (product == ceil(area)) {
            count++;
        }
        
		data = nextElement(&itr);
	}

    //Count all the circles with the area inside the groups.
    count += numGWithArea(img->groups, ceil(area), NULL, 'g', 'c');

    return count;
}

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data) {
    if (img == NULL)
        return 0;
    if (checkFileName(data) == NULL)
        return 0;

    int count = 0;
    ListIterator itr = createIterator(img->paths);
    void* node = nextElement(&itr);

    Path* current = (Path*) node;

    while (node != NULL)
	{
        //check if current node contains the data, if so, increment by 1
        current = (Path*) node;
        if (strcmp(current->data, data) == 0) {
            count++;
        }
        
		node = nextElement(&itr);
	}

    //Count all the paths with the data inside the groups.
    count += numGWithArea(img->groups, -1, data, 'g', 'p');

    return count;
}

//Get the number of atttributes for the current list. If you are given a rectangle, circle or path list, then call the function again
//   with the attribute list.
static int getAttrNum(List* list, char type) {
    if (list == NULL)
        return 0;
    int count = 0;

    List* temp = list;
    ListIterator itr = createIterator(temp);
    void* node = nextElement(&itr);

    while (node != NULL)
	{
        //Call again for the attributes of the type
        if (type == 'r') {
            Rectangle* r = (Rectangle*) node;
            count += getAttrNum(r->otherAttributes, 'a');
        } else if (type == 'c') {
            Circle* c = (Circle*) node;
            count += getAttrNum(c->otherAttributes, 'a');
        } else if (type == 'p') {
            Path* p = (Path*) node;
            count += getAttrNum(p->otherAttributes, 'a');
        //increase counter since we have an attribute here.
        } else if (type == 'a') {
            count++;
        }
        
		node = nextElement(&itr);
	}

    return count;
}

//A function to just recursively go through the group struct, and at each stage just call getAttrNum() to get the 
//  attributes of the rects, circles, paths, and groups. 
static int getAttrFromGroup(List* list) {
    if (list == NULL)
        return 0;
    int count = 0;

    List* temp = list;
    ListIterator itr = createIterator(temp);
    void* node = nextElement(&itr);

    while (node != NULL)
	{
        Group* g = (Group*) node;

        //Get attributes of everything but groups
        count += getAttrNum(g->rectangles, 'r');
        count += getAttrNum(g->circles, 'c');
        count += getAttrNum(g->paths, 'p');
        count += getAttrNum(g->otherAttributes, 'a');

        //get attributes of everything inside the group
        count += getAttrFromGroup(g->groups);

		node = nextElement(&itr);
	}

    return count;
}

// Function that returns the total number of Attribute structs in the SVGimage - i.e. the number of Attributes
//    contained in all otherAttributes lists in the structs making up the SVGimage
int numAttr(SVGimage* img) {
    if (img == NULL)
        return 0;
    int count = 0;

    //Get the count of all attributes in everything but groups
    count += getAttrNum(img->rectangles, 'r');
    count += getAttrNum(img->circles, 'c');
    count += getAttrNum(img->paths, 'p');
    count += getAttrNum(img->otherAttributes, 'a');

    //Call recursive function to get all the attributes in every struct in the list of groups
    count += getAttrFromGroup(img->groups);

    return count;
}

//Recursively go through each group and check the length of all the lists in each node.
int getSumWithLen(List* groups, int len) {
    if (groups == NULL)
        return 0;
    if (len < 0)
        return 0;
    int count = 0;
    int sum = 0;

    ListIterator itr = createIterator(groups);
    void* node = nextElement(&itr);

    Group* group = (Group*) node;

    while (node != NULL)
	{
        sum = 0;
        group = (Group*) node;
        
        sum += getLength(group->rectangles);
        sum += getLength(group->circles);
        sum += getLength(group->paths);
        sum += getLength(group->groups);
        if (sum == len)
            count++;

        if (getLength(group->groups) > 0) {
            count += getSumWithLen(group->groups, len);
        }
        
		node = nextElement(&itr);
	}
    
    return count;
}

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len) {
    if (img == NULL)
        return 0;
    if (len < 0)
        return 0;

    int count = 0;

    if (getLength(img->groups) > 0) {
        count = getSumWithLen(img->groups, len);
    }

    return count;
}

//How to free an Attribute struct
void deleteAttribute( void* data) {
    if (data == NULL)
        return;
    Attribute* attr = (Attribute*) data;
    free(attr->name);
    free(attr->value);
    free(attr);
}

//How to represent the data in an Attribute struct in terms of a string
char* attributeToString( void* data) {
    if (data == NULL)
        return NULL;
    Attribute* attr = (Attribute*) data;
    char* string = malloc(  strlen(attr->name) + 3 +
                            strlen(attr->value) + 3 +
                            + 1 );

    sprintf(string, "   %s = %s", attr->name, attr->value);

    return string;
}

//How to compare Attributes
int compareAttributes(const void *first, const void *second) {
    if (first == NULL || second == NULL)
        return 0;
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

//How to free a group struct
void deleteGroup(void* data) {
    if (data == NULL)
        return;
    Group* group = (Group*) data;

    clearList(group->rectangles);
    freeList(group->rectangles);

    clearList(group->circles);
    freeList(group->circles);

    clearList(group->paths);
    freeList(group->paths);

    clearList(group->otherAttributes);
    freeList(group->otherAttributes);

    clearList(group->groups);
    freeList(group->groups);

    free(group);
}

//How to represent the data in a Group struct in terms of a string
char* groupToString( void* data) {
    if (data == NULL)
        return NULL;
    Group* group = (Group*) data;
    List *group_attr = group->otherAttributes;
    char *group_attr_string = toString(group_attr);
    char *g_rect_attr_string = toString(group->rectangles);
    char *g_circle_attr_string = toString(group->circles);
    char *g_path_attr_string = toString(group->paths);
    char *g_g_attr_string = toString(group->groups);

    char* string = malloc(7 + (12 + strlen(group_attr_string) + 1) + (strlen(g_rect_attr_string) + 2) + (strlen(g_circle_attr_string) + 2) + (strlen(g_path_attr_string) + 2) + (strlen(g_g_attr_string) + 2 + 22 + 1));
    
    sprintf(string, "Group:\nAttributes:\n%s\n\t%s\n\t%s\n\t%s\n\t%s\n----------End of Group",
                group_attr_string, g_rect_attr_string, g_circle_attr_string, g_path_attr_string, g_g_attr_string);

    free(g_rect_attr_string);
    free(g_circle_attr_string);
    free(g_path_attr_string);
    free(g_g_attr_string);
    free(group_attr_string);
    return string;
}

//How to compare Groups
int compareGroups(const void *first, const void *second) {
    return 0;
}

//How to free a rectangle struct
void deleteRectangle(void* data) {
    if (data == NULL)
        return;
    Rectangle* rect = (Rectangle*) data;

    clearList(rect->otherAttributes);
    freeList(rect->otherAttributes);

    free(rect);
}

//How to represent the data in a Rectangle struct in terms of a string
char* rectangleToString(void* data) {
    if (data == NULL)
        return NULL;
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
    if (first == NULL || second == NULL)
        return 0;
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
    if (data == NULL)
        return;
    Circle *c = (Circle*) data;

    clearList(c->otherAttributes);
    freeList(c->otherAttributes);

    free(c);
}

//How to represent the data in a Circle struct in terms of a string
char* circleToString(void* data) {
    if (data == NULL)
        return NULL;
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
    if (first == NULL || second == NULL)
        return 0;
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
    if (data == NULL)
        return;
    Path *path = (Path*) data;

    free(path->data);
    clearList(path->otherAttributes);
    freeList(path->otherAttributes);

    free(path);
}

//How to represent the data in a Path struct in terms of a string
char* pathToString(void* data) {
    if (data == NULL)
        return NULL;
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
    if (first == NULL || second == NULL)
        return 0;
    const Path* one = (const Path*) first;
    const Path* two = (const Path*) second;

    return strcmp(one->data, two->data);
}


/** A2 Code **/

//Will validate the xmlDoc tree, and check if it complies with schemaFile given.
/**
 * Function uses functionality from http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html as highlighted in the assignment description.
 */
static bool validateXMLDoc(xmlDoc* doc, char* schemaFile) {
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    xmlLineNumbersDefault(1);

    ctxt = xmlSchemaNewParserCtxt(schemaFile);

    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    int ret = 0;
    xmlSchemaValidCtxtPtr ctxt_;

    ctxt_ = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(ctxt_, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    ret = xmlSchemaValidateDoc(ctxt_, doc);

    xmlSchemaFreeValidCtxt(ctxt_);
    xmlFreeDoc(doc);
    

    // free the resource
    if(schema != NULL)
        xmlSchemaFree(schema);

    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();

    if (ret == 0)
        return true;
    return false;
}

//Function will first check file names, then create an xmlDoc from that xml file. Validates it. Then creates an SVGimage from the xmlDoc tree struct.
SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
    char* svg = checkFileName(fileName);
    char* schema = checkFileName(schemaFile);
    if (svg == NULL|| schema == NULL) {
        return NULL;
    }

    //Get xmlDoc from file.
    xmlDoc* doc = getDocViaFile(svg);
    if (doc == NULL)
        return NULL;

    //Check if the xmlDoc is a valid xml file via the schema.
    bool response = validateXMLDoc(doc, schema);
    if (response == false)
        return NULL;

    //Create the SVGimage since the svg file is a valid svg file.
    SVGimage* image = createSVGimage(svg);

    return image;
}

//Converts a float number into a string. Will need to be freed in the calling function.
static unsigned char* floatToString(float value, char units[50]) {
    char* val = malloc(sizeof(float) * 3 + 3);

    sprintf(val, "%f", value); 

    strcat(val, units);
    return (unsigned char*) val;
}

//Will link two xmlAttr structs together by pointing them to eachother.
static void linkAttr(xmlAttr* first, xmlAttr* second) {
    if (first != NULL && second != NULL) {
        first->next = second;
        second->prev = first;
    } 
}

//This will create xmlAttr structs for each attribute in the attribute list. It will also connect them to the previous xmlAttr and its parent.
/** previous: The previous attribute that you want to link this to.
 *  attributes: A list of each and every attribute
 *  parent: The parent of this attribute **/
static bool xmlAddOtherAttributes(xmlAttr* previous, List* attributes, xmlNode* parent) {
    //Used for linking two xmlAttr's together
    xmlAttr* current = NULL;
    xmlAttr* prev = previous;

    //Pointer to attribute list, just to be safe
    List* attrs = attributes;

    //Iterate through the attribute list
    ListIterator itr = createIterator(attrs);
    void* cur = nextElement(&itr);

    while (cur != NULL)
	{
        Attribute* a = (Attribute*) cur;
        // printf("gonna add %s -> %s\n", a->name, a->value);
        if (a->name == NULL || a->value == NULL) {
            current = xmlNewProp(parent, (unsigned char*) "testetst", (unsigned char*) a->value);
            // return false;
        }
        //Create new attribute prop and link it to its siblings
        current = xmlNewProp(parent, (unsigned char*) a->name, (unsigned char*) a->value);

        //Link this attribute with its sibling
        linkAttr(prev, current);
        
        //Set prev to this node for linking its younger sibling
        prev = current;

        //Go to the next element
        cur = nextElement(&itr);
	}
    return true;
}

//Will add the attributes for the given xmlNode from the corresponding shape.
static bool addxmlAttr(void* val, char type, xmlNode* node) {
    //These are used to interchangingly for connecting the attributes together
    xmlAttr* one= NULL;
    xmlAttr* two = NULL;
    
    bool result = true;

    //Setup the attribute list if the value is a rectangle
    if (type == 'r') {
        Rectangle* r = (Rectangle*) val;
        if (r == NULL) return false;
        if (r->otherAttributes == NULL) return false;
        //Set the x attribute
        unsigned char *x = floatToString(r->x, r->units);
        two = xmlNewProp(node, (unsigned char *)"x", x);
        free(x);

        //Set and link the y attribute
        unsigned char *y = floatToString(r->y, r->units);
        one = xmlNewProp(node, (unsigned char *)"y", y);
        free(y);
        linkAttr(two,one);

        //Set and link the height attribute
        unsigned char *height = floatToString(r->height, r->units);
        two = xmlNewProp(node, (unsigned char *)"height", height);
        free(height);
        linkAttr(one, two);

        //Set and link the width attribute
        unsigned char *width = floatToString(r->width, r->units);
        one = xmlNewProp(node, (unsigned char *)"width", width);
        free(width);
        linkAttr(two, one);

        //Link all the rest of the attributes.
        result = xmlAddOtherAttributes(one, r->otherAttributes, node);

    } 
    //Setup the attribute list if the value is a circle
    else if (type == 'c') {
        Circle* c = (Circle *) val;
        if (c == NULL) return false;
        if (c->otherAttributes == NULL) return false;
        //Set the cx
        unsigned char* cx = floatToString(c->cx, c->units);
        one = xmlNewProp(node, (unsigned char *)"cx", cx);
        free(cx);

        //Set the cy
        unsigned char* cy = floatToString(c->cy, c->units);
        two = xmlNewProp(node, (unsigned char *)"cy", cy);
        free(cy);
        linkAttr(one, two);

        //Set the r
        unsigned char* r = floatToString(c->r, c->units);
        one = xmlNewProp(node, (unsigned char *)"r", r);
        free(r);
        linkAttr(two,one);

        result = xmlAddOtherAttributes(one, c->otherAttributes, node);
    } 
    //Setup the attribute list if the value is a path
    else if (type == 'p') {
        Path* p = (Path*) val;
        if (p == NULL) return false;
        if (p->data == NULL || p->otherAttributes == NULL) return false;
        //Set the data
        one = xmlNewProp(node,(unsigned char*)"d", (unsigned char*)p->data);
        
        result = xmlAddOtherAttributes(one, p->otherAttributes, node);
    } 
    //Setup the attribute list if the value is a group
    else if (type == 'g') {
        Group* g = (Group*) val;
        if (g == NULL) return false;
        if (g->rectangles == NULL || g->circles == NULL || g->paths == NULL || g->groups == NULL || g->otherAttributes == NULL) return false;

        result = xmlAddOtherAttributes(NULL, g->otherAttributes, node);
    }
    return result;
}

//Add the entire lists content onto the link with the olderSibling
static bool xmlAddList(List* list, char type, xmlNode* olderSibling, xmlNs* ns) {
    if (list == NULL || olderSibling == NULL || ns == NULL)
        return false;
    xmlNode* node = NULL;
    xmlNode* prev = olderSibling;
    bool result = true;
    bool overall = true;

    //Iterate through the list
    List* temp = list;
    ListIterator itr = createIterator(temp);
    void* cur = nextElement(&itr);

    while (cur != NULL)
	{
        //Create a node and set the namespace for it.
        node = xmlNewNode(NULL, (unsigned char*)"");
        xmlSetNs(node,ns);
        result = addxmlAttr(cur, type, node);
        // if (!result) overall = false;

        //Add each rectangle to the link
        if (type == 'r') {
            xmlNodeSetName(node, (unsigned char*)"rect");
        } 
        //Add each circle to the link
        else if (type == 'c') {
            xmlNodeSetName(node,(unsigned char*)"circle");
        } 
        //Add each path to the link
        else if (type == 'p') {
            xmlNodeSetName(node,(unsigned char*)"path");
        }
        //Add each group to the link
        else if (type == 'g') {
            Group* g = (Group*) cur;
            xmlNodeSetName(node, (unsigned char*)"g");
            
            //Used to have a default child to attach links to.(since groups has no default attribute to attach other attributes to)
            //This will get removed later, just used as a start of the link.
            xmlNode* child = xmlNewChild(node, NULL, (unsigned char*)"text", (unsigned char*)"");

            result = xmlAddList(g->rectangles, 'r', child, ns);
            if (!result) overall = false;
            result = xmlAddList(g->circles,'c', child, ns);
            if (!result) overall = false;
            result = xmlAddList(g->paths, 'p', child, ns);
            if (!result) overall = false;
            result = xmlAddList(g->groups, 'g', child, ns);
            if (!result) overall = false;
            
            //Free the node that was used as a start of the link for each groups children
            //Get the node from the lists if there was lists added.
            xmlNode* removeText = xmlNextElementSibling(child);
            //Change the pointers to not have anything to do with the child node anymore.
            if (removeText != NULL) {
                node->children = removeText;
                removeText->parent = node;
                removeText->prev = NULL;
            }
            xmlFreeNode(child);
        }
        //Add this node onto its sibling.
        xmlAddSibling(prev,node);
        //Set the next sibling pointer
        prev = node;
        //Go to the next element in the list.
		cur = nextElement(&itr);
	}
    return overall;
}

static void freeTempChild(xmlNode* root, xmlNode* child) {
    //Free the node that was used as a start of the link for each groups children
    //Get the node from the lists if there was lists added.
    xmlNode* removeText = xmlNextElementSibling(child);
    //Change the pointers to not have anything to do with the child node anymore.
    if (removeText != NULL) {
        root->children = removeText;
        removeText->parent = root;
        removeText->prev = NULL;
    }
    xmlFreeNode(child);
}

//This function will create a xmlDoc based off of the SVGimage struct.
static xmlDoc* svgToDoc(SVGimage* image) {
    if (image == NULL || image->rectangles == NULL || image->circles == NULL|| image->paths == NULL|| image->groups == NULL|| image->otherAttributes == NULL)
        return NULL;

    bool result = true;
    bool overall = true;

    //Initialize xmlDoc and declare root pointer for the tree
    xmlDoc* doc = xmlNewDoc(NULL);
    xmlNode* root = NULL;

    //Set root node(svg node) and create the namespace node
    root = xmlNewNode(NULL, (const unsigned char*) "svg");
    xmlNs* ns = xmlNewNs(root, (const unsigned char*)(image->namespace), NULL);
    
    //Set the namespace of the root node
    xmlSetNs(root, ns);

    //Add the svg attributes
    result = xmlAddOtherAttributes(NULL, image->otherAttributes, root);
    if (!result) overall = false;

    //Make the "svg" node the root element in the xmlDoc.
    xmlDocSetRootElement(doc, root);
    
    //Used to have a default child to attach links to. Since there may not be a title or description.
    //This will get removed later, just used as a start of the link.
    xmlNode* child = xmlNewChild(root, NULL, (unsigned char*)"text", (unsigned char*)"");

    //Add the title and description to the children of the root
    if (strlen(image->title) > 0) 
        xmlNewTextChild(root, NULL, (const unsigned char*)"title", (unsigned char*)image->title);
    
    if (strlen(image->description) > 0)
        xmlNewTextChild(root, NULL, (const unsigned char*)"desc", (unsigned char*)image->description);

    //Add the rectangle list to the nodes link.
    result = xmlAddList(image->rectangles, 'r', child, ns);
    if (!result) overall = false;

    //Add the circle list to the nodes link.
    result = xmlAddList(image->circles, 'c', child, ns);
    if (!result) overall = false;

    //Add the path list to the nodes link.
    result = xmlAddList(image->paths, 'p', child, ns);
    if (!result) overall = false;
    
    //Add the group list to the nodes link.
    result = xmlAddList(image->groups, 'g', child, ns);
    if (!result) overall = false;

    freeTempChild(root,child);

    if (!overall) {
        xmlFreeDoc(doc);
        printf("got rekt\n");
        xmlCleanupParser();
        xmlMemoryDump();
        return NULL;
    }

    return doc;
}

//This function will make sure that the SVGimage struct is valid via the schemaFile that is provided.
bool validateSVGimage(SVGimage* image, char* schemaFile) {
    if (image == NULL || schemaFile == NULL)
        return false;

    xmlDoc* doc = svgToDoc(image);
    if (doc == NULL)
        return false;
    
    bool answer = validateXMLDoc(doc, schemaFile);
    
    return answer;
}

//This will write an SVGimage struct to file with the name that was given in fileName.
//Will return true on success and false on failure
bool writeSVGimage(SVGimage* image, char* fileName) {
    char* file = checkFileName(fileName);
    if (image == NULL || file == NULL)
        return false;

    xmlDoc* doc = svgToDoc(image);
    if (doc == NULL)
        return false;

    int success = xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);
    
    xmlFreeDoc(doc);
    xmlCleanupParser();

    if (success < 0)
        return false;

    return true;
}

//Will change the 'toBeChanged' into the value in attr, and delete it.
//Works for changing float and strings, all dependent on what 'num'.
//NULL -> string 'toBeChanged'
//!NULL -> float 'num'
void changeValue(char* toBeChanged, Attribute* attr, float* num) {
    if (num == NULL) {
        strcpy(toBeChanged,attr->value);
    } else {
        char units[50];
        *num = stringToFloat(attr->value, units);
    }
    deleteAttribute(attr);
}

//Will go through a list of attributes and either change an existing one, or add a new one
//@var list <- MUST BE A LIST OF ATTRIBUTES
void changeAttr(List* list, Attribute* attr) {
    if (list == NULL || attr == NULL)
        return;
    ListIterator itr = createIterator(list);
    void* node = nextElement(&itr);

    int i = 0;;
    for (i = 0; node != NULL; i++)
	{
        Attribute* cur = (Attribute*) node;
        if (strcmp(cur->name,attr->name) == 0) {
            changeValue(cur->value,attr, NULL);
            return;
        }
        node = nextElement(&itr);
	}
    // if (i == 0)
    //     deleteAttribute(attr);
    insertBack(list,attr);
}

//Go through the list and add the attribute to the specified location in the list
void changeAttrInList(List* list, int elemIndex, Attribute* newAttribute, char type) {
    ListIterator itr = createIterator(list);
    void* node = nextElement(&itr);

    int i = 0;
    for (i = 0; node != NULL; i++) {
        if (i == elemIndex) {
            if (type == 'r') {
                Rectangle* r = (Rectangle*) node;
                if (r->otherAttributes == NULL) {
                    deleteAttribute(newAttribute);
                    return;
                }
                if (strcmp(newAttribute->name, "x") == 0) {
                    changeValue(NULL, newAttribute, &(r->x));
                } else if (strcmp(newAttribute->name, "y") == 0) {
                    changeValue(NULL, newAttribute, &(r->y));
                } else if (strcmp(newAttribute->name, "width") == 0) {
                    changeValue(NULL, newAttribute, &(r->width));
                } else if (strcmp(newAttribute->name, "height") == 0) {
                    changeValue(NULL, newAttribute, &(r->height));
                } else {
                    //Other attributes
                    changeAttr(r->otherAttributes, newAttribute);
                }
            } else if (type == 'c') {
                Circle* c = (Circle*) node;
                if (c->otherAttributes == NULL) {
                    deleteAttribute(newAttribute);
                    return;
                }
                if (strcmp(newAttribute->name, "cx") == 0) {
                    changeValue(NULL, newAttribute, &(c->cx));
                } else if (strcmp(newAttribute->name, "cy") == 0) {
                    changeValue(NULL, newAttribute, &(c->cy));
                } else if (strcmp(newAttribute->name, "r") == 0) {
                    changeValue(NULL, newAttribute, &(c->r));
                } else {
                    //Other attributes
                    changeAttr(c->otherAttributes, newAttribute);
                }

            } else if (type == 'p') {
                Path* p = (Path*) node;
                if (p->otherAttributes == NULL) {
                    deleteAttribute(newAttribute);
                    return;
                }
                if (strcmp(newAttribute->name, "d") == 0 || strcmp(newAttribute->name, "data") == 0) {
                    changeValue(p->data, newAttribute, NULL);
                } else {
                    //Other attributes
                    changeAttr(p->otherAttributes, newAttribute);
                }
            } else if (type == 'g') {
                Group* g = (Group*) node;
                if (g->otherAttributes == NULL) {
                    deleteAttribute(newAttribute);
                    return;
                }
                changeAttr(g->otherAttributes, newAttribute);
            }
        }
        node = nextElement(&itr);
	}
    if (i == 0)
        deleteAttribute(newAttribute);
}

//Will add an attribute to the corresponding component/list. This function will call the changeAttrInList to find the attribute in the list specified.
void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute) {
    if (image == NULL || newAttribute == NULL || elemType > 4 || elemType < 0)
        return; 
    if (newAttribute->name == NULL || newAttribute->value == NULL)
        return;
    if (elemType == SVG_IMAGE) {
        if (strcmp(newAttribute->name, "title") == 0) {
            changeValue(image->title, newAttribute, NULL);
        } else if (strcmp(newAttribute->name, "description") == 0 || strcmp(newAttribute->name, "desc") == 0) {
            changeValue(image->description, newAttribute, NULL);
        } else {
            //Other attributes
            changeAttr(image->otherAttributes, newAttribute);
        }
    } else if (elemType == CIRC) {
        changeAttrInList(image->circles, elemIndex, newAttribute, 'c');        
    } else if (elemType == RECT) {
        changeAttrInList(image->rectangles, elemIndex, newAttribute, 'r');  
    } else if (elemType == PATH) {
        changeAttrInList(image->paths, elemIndex, newAttribute, 'p');
    } else if (elemType == GROUP) {
        changeAttrInList(image->groups, elemIndex, newAttribute, 'g');
    }
}

//Will add either a rectangle, circle, or path into the SVGimage.
void addComponent(SVGimage* image, elementType type, void* newElement) {
    if (image == NULL || type < 1 || type > 3 || newElement == NULL) 
        return;
    if (image->rectangles == NULL || image->circles == NULL || image->paths == NULL)
        return;
    if (type == RECT) {
        Rectangle* r = (Rectangle*) newElement;
        if (r->otherAttributes == NULL) return;
        insertBack(image->rectangles, r);
    } else if (type == CIRC) {
        Circle* c = (Circle*) newElement;
        if (c->otherAttributes == NULL) return;
        insertBack(image->circles, c);
    } else if (type == PATH) {
        Path* p = (Path*) newElement;
        if (p->otherAttributes == NULL) return;
        insertBack(image->paths, p);
    }
}

//Return the null string for the JSON strings.
//Data must not be more than 2 characters!!!
char* nullReturnValueJSON(char* data) {
    char* string = malloc(3);
    strcpy(string,data);
    return string;
}

//Convert an attribute to a JSON string.
char* attrToJSON(const Attribute *a) {
    if (a == NULL)
        return nullReturnValueJSON("{}");

    char* string = malloc(22 + strlen(a->name) + strlen(a->value) + 1);

    sprintf(string, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return string;
}

//Convert a circle to a JSON string.
char* circleToJSON(const Circle *c) {
    if (c == NULL)
        return nullReturnValueJSON("{}");

    char* string = malloc(40 + (sizeof(float) * 4) + sizeof(int) + strlen(c->units) + 1);

    sprintf(string, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", c->cx, c->cy, c->r, getAttrNum(c->otherAttributes,'a'), c->units);

    return string;
}

//Converts a rectangle to a JSON string.
char* rectToJSON(const Rectangle *r) {
    if (r == NULL)
        return nullReturnValueJSON("{}");

    char* string = malloc(43 + (sizeof(float) * 4 * 4) + sizeof(int) + strlen(r->units) + 1);

    sprintf(string, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", r->x, r->y, r->width, r->height, getAttrNum(r->otherAttributes, 'a'), r->units);

    return string;
}

//Converts a path to a JSON string.
char* pathToJSON(const Path *p) {
    if (p == NULL)
        return nullReturnValueJSON("{}");

    char *string = malloc(19 + strlen(p->data) + sizeof(int) + 1);

    sprintf(string, "{\"d\":\"%s\",\"numAttr\":%d}", p->data, getAttrNum(p->otherAttributes, 'a'));

    return string;
}

//Returns the number of immediate children in Group.
static int getImmediateChildCount(List* list) {
    if (list == NULL)
        return 0;
    int count = 0;
    ListIterator itr = createIterator(list);
    void* node = nextElement(&itr);

    while (node != NULL) {
        count++;
        node = nextElement(&itr);
    }
    return count;
}

//Converts a group to a JSON string.
char* groupToJSON(const Group *g) {
    if (g == NULL)
        return nullReturnValueJSON("{}");

    int children = getImmediateChildCount(g->rectangles);
    children += getImmediateChildCount(g->circles);
    children += getImmediateChildCount(g->paths);
    children += getImmediateChildCount(g->groups);

    char* string = malloc(24 + (sizeof(int) * 2) + 1);

    sprintf(string, "{\"children\":%d,\"numAttr\":%d}", children, getAttrNum(g->otherAttributes, 'a'));

    return string;
}

//Will return the JSON string for the specified list of elements.
static char* getJSONString(List* list, char type) {
    if (list == NULL)
        return "";

    char* string = malloc(3);
    string[0] = '\0';
    strcat(string, "[");

    ListIterator itr = createIterator(list);
    void* node = nextElement(&itr);
    
    while (node != NULL) {
        char* cur = NULL;
        if (type == 'r') {
            Rectangle* r = (Rectangle*) node;
            cur = rectToJSON(r);
        } else if (type == 'c') {
            Circle* c = (Circle*) node;
            cur = circleToJSON(c);
        } else if (type == 'p') {
            Path* p = (Path*) node;
            cur = pathToJSON(p);
        } else if (type == 'a') {
            Attribute* a = (Attribute*) node;
            cur = attrToJSON(a);
        } else if (type == 'g') {
            Group* g = (Group*) node;
            cur = groupToJSON(g);
        }

        string = realloc(string, strlen(cur) + strlen(string) + 3 + 1);
        strcat(string, cur);
        free(cur);

        node = nextElement(&itr);
        if (node != NULL)
            strcat(string, ",");
    }
    strcat(string, "]");
    return string;
}

//Get the JSON string for a list of attributes.
char* attrListToJSON(const List *list) {
    if (list == NULL) 
        return nullReturnValueJSON("[]");

    char* string = getJSONString((List*)list, 'a');

    return string;
}

//Get the JSON string for a list of circles.
char* circListToJSON(const List *list) {
    if (list == NULL)
        return nullReturnValueJSON("[]");

    char* string = getJSONString((List*)list, 'c');

    return string;
}

//Get the JSON string for a list of rectangles.
char* rectListToJSON(const List *list) {
    if (list == NULL)
        return nullReturnValueJSON("[]");

    char* string = getJSONString((List*)list, 'r');

    return string;
}

//Get the JSON string for a list of paths.
char* pathListToJSON(const List *list) {
    if (list == NULL)
        return nullReturnValueJSON("[]");
    
    char* string = getJSONString((List*)list, 'p');

    return string;
}

//Get the JSON string for a list of groups.
char* groupListToJSON(const List *list) {
    if (list == NULL)
        return nullReturnValueJSON("[]");
    
    char* string = getJSONString((List*)list, 'g');

    return string;
}

//Get the JSON string for an entire SVGimage;
char* SVGtoJSON(const SVGimage* imge) {
    if (imge == NULL) 
        return nullReturnValueJSON("{}");

    List* rects = getRects((SVGimage*)imge);
    int rNum = getImmediateChildCount(rects);
    freeList(rects);
    
    List* circles = getCircles((SVGimage*)imge);
    int cNum = getImmediateChildCount(circles);
    freeList(circles);

    List* paths = getPaths((SVGimage*)imge);
    int pNum = getImmediateChildCount(paths);
    freeList(paths);

    List* groups = getGroups((SVGimage*)imge);
    int gNum = getImmediateChildCount(groups);
    freeList(groups);

    char* string = malloc(48 + (sizeof(int) * 4) + 1);

    sprintf(string, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", rNum, cNum, pNum, gNum);

    return string;
}


//Will convert a JSON string to a SVGimage.
SVGimage* JSONtoSVG(const char* svgString) {
    if (svgString == NULL) 
        return NULL;
    
    SVGimage* image = malloc(sizeof(SVGimage));
    strcpy(image->namespace, "http://www.w3.org/2000/svg");
    image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    char c;
    bool lever = false;
    bool gotTitle = false;
    bool gotDescription = false;
    
    int j = 0;
    //Iterate through the string
    for (int i = 0; i < strlen(svgString); i++) {
        //If you got title and description. Return.
        if (gotTitle && gotDescription)
            break;
        //Get the current character
        c = svgString[i];
        //if you found a ':' then prepare for getting a value. Remove the next '"', switch the lever to tell you to start getting the letters, reset j, run loop again.
        if (c == ':') {
            i++;
            c = svgString[i];
            if (c == '\"') {
                lever = true;
                j = 0;
                continue;
            }
        }
        //Currently on get data mode
        if (lever) {
            //If done get data mode, then flip the lever. and swap the corresponding gotTitle and gotDescription
            if (c == '\"') {
                lever = false;
                if (!gotTitle)
                    gotTitle = true;
                else if (!gotDescription)
                    gotDescription = true;
                continue;
            }
            //Put char into current string 
            if (!gotTitle) {
                (image->title)[j] = c;
            } else {
                (image->description)[j] = c;
            }                
        }
        j++;
    }
    return image;
}

//Will convert a JSON string into a rectangle.
Rectangle* JSONtoRect(const char* svgString) {
    if (svgString == NULL)
        return NULL;

    Rectangle* r = malloc(sizeof(Rectangle));
    r->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    
    char c;
    bool lever = false;
    bool gotX = false;
    bool gotY = false;
    bool gotW = false;
    bool gotH = false;
    bool gotUnits = false;
    char* x = malloc(20);
    char* y = malloc(20);
    char* w = malloc(20);
    char* h = malloc(20);
    char* units = malloc(20);
    
    int j = 0;
    //Iterate through the string
    for (int i = 0; i < strlen(svgString); i++) {
        //If you got all the data. Return.
        if (gotX && gotY && gotW && gotH && gotUnits)
            break;

        //Get the current character
        c = svgString[i];

        //if you found a ':' then prepare for getting a value. Remove the next '"' if applicable, switch the lever to tell you to start getting the letters, reset j, run loop again(if next is units).
        if (c == ':') {
            i++;
            c = svgString[i];
            lever = true;
            j = 0;
            if (gotX && gotY && gotW && gotH)
                if (c == '\"') {
                    continue;
                }
            
        }
        //Currently on get data mode
        if (lever) {
            //If done get data mode, then flip the lever. and swap the corresponding boolean values
            if (c == '\"') {
                lever = false;
                if (!gotX)
                    gotX = true;
                else if (!gotY)
                    gotY = true;
                else if (!gotW)
                    gotW = true;
                else if (!gotH)
                    gotH = true;
                else if (!gotUnits)
                    gotUnits = true;
                continue;
            }
            //Put char into current string 
            if (!gotX)
                x[j] = c;
            else if (!gotY)
                y[j] = c;
            else if (!gotW) 
                w[j] = c;
            else if (!gotH)
                h[j] = c;
            else if (!gotUnits)
                units[j] = c;        
        }
        j++;
    }

    r->x = atof(x);
    r->y = atof(y);
    r->height = atof(h);
    r->width = atof(w);
    strcpy(r->units, units);

    free(x);
    free(y);
    free(h);
    free(w);
    free(units);

    return r;
}

//Will convert a JSON string into a Circle.
Circle* JSONtoCircle(const char* svgString) {
    if (svgString == NULL)
        return NULL;
    
    Circle* circ = malloc(sizeof(Circle));
    circ->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    char c;
    bool lever = false;
    bool gotCX = false;
    bool gotCY = false;
    bool gotR = false;
    bool gotUnits = false;
    char* cx = malloc(20);
    char* cy = malloc(20);
    char* r = malloc(20);
    char* units = malloc(20);
    
    int j = 0;
    //Iterate through the string
    for (int i = 0; i < strlen(svgString); i++) {
        //If you got all the data. Return.
        if (gotCX && gotCY && gotR && gotUnits)
            break;

        //Get the current character
        c = svgString[i];

        //if you found a ':' then prepare for getting a value. Remove the next '"' if applicable, switch the lever to tell you to start getting the letters, reset j, run loop again(if next is units).
        if (c == ':') {
            i++;
            c = svgString[i];
            lever = true;
            j = 0;
            if (gotCX && gotCY && gotR)
                if (c == '\"') {
                    continue;
                }
            
        }
        //Currently on get data mode
        if (lever) {
            //If done get data mode, then flip the lever. and swap the corresponding boolean values
            if (c == '\"') {
                lever = false;
                if (!gotCX)
                    gotCX = true;
                else if (!gotCY)
                    gotCY = true;
                else if (!gotR)
                    gotR = true;
                else if (!gotUnits)
                    gotUnits = true;
                continue;
            }
            //Put char into current string 
            if (!gotCX)
                cx[j] = c;
            else if (!gotCY)
                cy[j] = c;
            else if (!gotR) 
                r[j] = c;
            else if (!gotUnits)
                units[j] = c;        
        }
        j++;
    }

    circ->cx = atof(cx);
    circ->cy = atof(cy);
    circ->r = atof(r);
    strcpy(circ->units, units);

    free(cx);
    free(cy);
    free(r);
    free(units);

    return circ;
}

char* titleToJSON(SVGimage* img) {
    if (img == NULL) {
        return nullReturnValueJSON("{}");
    }
    char *title = malloc(strlen(img->title) * sizeof(char) + 1 + 13);

    sprintf(title, "{\"title\":\"%s\"}", img->title);

    return title;
}

char* descToJSON(SVGimage* img) {
    if (img == NULL) {
        return nullReturnValueJSON("{}");
    }
    char* desc = malloc(strlen(img->description) * sizeof(char) + 1 + 12);

    sprintf(desc, "{\"desc\":\"%s\"}", img->description);

    return desc;
}

char* getSVGJSON(char* file) {
    const SVGimage* img = createValidSVGimage(file, "svg.xsd");

    char* arr = SVGtoJSON(img);

    deleteSVGimage((SVGimage*) img);
    return arr;
}

bool checkIfValid(char* file) {
    SVGimage* img = createValidSVGimage(file, "svg.xsd");
    if (img == NULL) {
        deleteSVGimage(img);
        return false;
    } else {
        deleteSVGimage(img);
        return true;
    }
}

char* getTitleJSON(char* file) {
    SVGimage *img = createValidSVGimage(file, "svg.xsd");
    char *title = malloc(strlen(img->title) + 1);
    strcpy(title, img->title);

    deleteSVGimage(img);
    return title;
}

char* getDescJSON(char* file) {
    SVGimage* img = createValidSVGimage(file, "svg.xsd");
    char* description = malloc(strlen(img->description) + 1);
    strcpy(description, img->description);

    deleteSVGimage(img);
    return description;
}

char* getRectsJSON(char* file) {
    SVGimage* img = createValidSVGimage(file, "svg.xsd");

    char *rects = rectListToJSON(img->rectangles);

    deleteSVGimage(img);
    return rects;
}

char *getCircsJSON(char* file) {
    SVGimage *img = createValidSVGimage(file, "svg.xsd");

    char *circles = circListToJSON(img->circles);

    deleteSVGimage(img);
    return circles;
}

char *getPathsJSON(char* file) {
    SVGimage *img = createValidSVGimage(file, "svg.xsd");

    char *paths = pathListToJSON(img->paths);

    deleteSVGimage(img);
    return paths;
}

char* getGroupsJSON(char* file) {
    SVGimage *img = createValidSVGimage(file, "svg.xsd");

    char *groups = groupListToJSON(img->groups);
    
    deleteSVGimage(img);
    return groups;
}