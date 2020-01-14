#include "../include/SVGParser.h"
#include "../include/LinkedListAPI.h"
#include "assert.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

/*int goodStringCompare(char *first, char *second) {
    char *one = first;
    char *two = second;
    return strcmp(one,two);
}*/

char* checkFileName(char *file) {
    if (file == NULL)
        return NULL;
    
    if (strlen(file) == 0)
        return NULL;

    if(file[0] == 0 && file[strlen(file)] == 0) 
        return NULL;

    return file;
}

xmlDoc* getDocViaFile(char *fileName) {
    xmlDoc* doc = NULL;

    LIBXML_TEST_VERSION
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
    }

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
const unsigned char* checkTitle(xmlNode* titleNode) {
    //TODO: also make sure if you find 1 title, dont do another one. Dont quote me on that. LOok it up.
    if (titleNode == NULL)
        return NULL;
    xmlNode* node = titleNode;
    const unsigned char* title = node->content;
    return checkLengthChar(title);
}

const unsigned char* checkDescription(xmlNode* descNode) {
    if (descNode == NULL)
        return NULL;
    xmlNode *node = descNode;
    const unsigned char* desc = node->content; 
    return checkLengthChar(desc);
}

//Basically get the namespace string from the node
const unsigned char* checkNameSpace(xmlNs* namespace) {
    if (namespace == NULL)
        return NULL;
    xmlNs* value = namespace;
    const unsigned char* URL = value->href;

    return checkLengthChar(URL);
}

void print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
    int i = 0, j = 0;
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        i++;
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        // Uncomment the code below if you want to see the content of every node.

        if (cur_node->content != NULL ){
            printf("  content: %s\n", cur_node->content);
        }

        // Iterate through every attribute of the current node
        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            printf("%d %d\n", i, ++j);
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);
            printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
        }
        
        print_element_names(cur_node->children);
    }
    printf("FINAL i = %d, j = %d\n", i ,j);
}

void addAttribute(const unsigned char* name, const unsigned char* value, List* listOfAttributes) {

}

SVGimage* convertXMLtoSVG(xmlNode* a_node, SVGimage* image) {
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
            printf("Current Loop name: [%s]\n", cur_node->name);

            //If cur_node->nsDef != NULL -> deal with namespace
            //Add and check the NAMESPACE to the SVGimage ----
            if (checkNameSpace(cur_node->nsDef) != NULL) {
                const unsigned char* ns = checkNameSpace(cur_node->nsDef);
                strcpy(image->namespace, (char*) ns);
                printf("Got the namespace. it is [%s]\n", image->namespace);

            //-------------
            }
        
            xmlNode *paren = cur_node->parent;

            //if name = title
            //Add and check the TITLE to the SVGimage -------
            if (paren->name !=NULL) {
                if (strcmp((char*)paren->name, (char*)"title") == 0) {
                    if (checkTitle(cur_node) != NULL) {
                        const unsigned char* title = checkTitle(cur_node);
                        strcpy(image->title, (char*) title);
                        printf("Got the title. it is [%s]\n", image->title);
                    }
                }
                //---------------------


                //if name = desc
                //Add and check the DESCRIPTION to the SVGimage ------
                else if (strcmp((char*)paren->name, (char*)"desc") == 0) {
                    if (checkDescription(cur_node) != NULL) {
                        const unsigned char* desc = checkDescription(cur_node);
                        strcpy(image->description, (char*) desc);
                        printf("Got the description. it is [%s]\n", image->description);
                    }
                }
            //--------------
            }

            //if parent name = svg
                if (strcmp((char*)cur_node->name,(char*)"svg") == 0) {
                    xmlAttr *attr_node = NULL;
                    //Add the other attribute of the svg.
                    for (attr_node = cur_node->properties; attr_node != NULL; attr_node = attr_node->next) {
                        xmlNode* alsoTemp = attr_node->children;
                        List* svgImageAttributes = image->otherAttributes;
                        printf("svg attributes are: Name = %s, Content = %s\n", attr_node->name, alsoTemp->content);                        
                        addAttribute(attr_node->name, alsoTemp->content, svgImageAttributes);
                    }
                    
                }

            //if name = rect

            //if name = circle

            //if name = path

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

SVGimage* makeSVG(xmlNode *a_node) {
    SVGimage *image = malloc(sizeof(SVGimage));

    SVGimage *fullSVG = convertXMLtoSVG(a_node, image); 

    return fullSVG;
}

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
    SVGimage* image = makeSVG(root);
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
    
    string = malloc(  strlen((char*)(img->namespace)) + 13 
                    + strlen((char*)(img->title)) + 9 
                    + strlen((char*)(img->description)) + 15
                    + 1);

    sprintf(string, "Namespace = %s\nTitle = %s\nDescription = %s\n", img->namespace, img->title, img->description);

    return string;
}

void deleteSVGimage(SVGimage* img) {
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
    printf("Sup.");
}

int main() {
    char* file = malloc(20);
    strcpy(file, "quad01.svg");
    //strcpy(file, "Emoji_poo.svg");
    //strcpy(file, "rect.svg");
    SVGimage* img = createSVGimage(file);

    deleteSVGimage(img);

    printf("Makefile Test.\n");
    
    free(file);
    return 0;
}
