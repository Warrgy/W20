/*
 * Assignment 1
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

//Checks to make sure the char can fit in a char[256] array
static const unsigned char* checkLengthChar(const unsigned char *string) {
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
            if (checkNameSpace(cur_node->nsDef) != NULL) {
                const unsigned char* ns = checkNameSpace(cur_node->nsDef);
                strcpy(image->namespace, (char*) ns);
            }
            //-------------
        
            xmlNode *paren = cur_node->parent;

            //if name = title
            //Add and check the TITLE to the SVGimage -------
            if (paren->name !=NULL) {
                if (strcmp(checkUppLowCase((char*)paren->name), (char*)"title") == 0) {
                    if (checkTitle(cur_node) != NULL) {
                        strcpy(image->title, checkTitle(cur_node));
                    }
                }
                //---------------------


                //if name = desc
                //Add and check the DESCRIPTION to the SVGimage ------
                else if (strcmp(checkUppLowCase((char*)paren->name), (char*)"desc") == 0) {
                    if (checkDescription(cur_node) != NULL) {
                        strcpy(image->description, checkDescription(cur_node));
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

// Function that returns a list of all rectangles in the image.
List* getRects(SVGimage* img) {
    if (img == NULL)
        return NULL;
    List* rects = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    
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
    List* circ = initializeList(&circleToString, &deleteCircle, &compareCircles);

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
    List* group = initializeList(&groupToString, &deleteGroup, &compareGroups);

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
    List* paths = initializeList(&pathToString, &deletePath, &comparePaths);

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
            count += numGWithArea(g->paths, -1, NULL, 'p', wanted);
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
bool validateXMLDoc(xmlDoc* doc, char* schemaFile) {
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

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
    char* svg = checkFileName(fileName);
    char* schema = checkFileName(schemaFile);
    if (svg == NULL|| schema == NULL) {
        return NULL;
    }

    xmlDoc* doc = getDocViaFile(svg);
    if (doc == NULL)
        return NULL;

    bool response = validateXMLDoc(doc, schema);
    if (response == false)
        return NULL;

    SVGimage* image = createSVGimage(svg);

    return image;
}

bool writeSVGimage(SVGimage* image, char* fileName) {
    return false;
}

bool validateSVGimage(SVGimage* image, char* schemaFile) {
    return false;
}

int main() {
    SVGimage* img = createValidSVGimage("rect.svg", "svg.xsd");
    printf("img = %p\n", (void*) img);
    deleteSVGimage(img);

    printf("Done.\n");
    return 0;
}