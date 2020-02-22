/* 
 * Slightly more complex example - list contents are a simple struct
 */
#include <stdio.h>
#include <string.h>
#include "LinkedListAPI.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct name {
	char* firstName;
	char* lastName;
	unsigned int age;
}Name;

//printFunc will return a string that contains a humanly readable copy of the list contents
char* printFunc(void *toBePrinted){
	
	char* tmpStr;
	Name* tmpName;
	int len;
	
	if (toBePrinted == NULL){
		return NULL;
	}
	
	tmpName = (Name*)toBePrinted;
		
	/*
	Length of the string is:
	length of the first and last names+3 spaces+4 chars for "Age:"+1 character for '\0'+20 digits to represent the age
	While we only need 3 digits to represent the human age, if the age is uninitialised, it can be any int value
	An int is 4 bytes, and needs up to 20 chars to represent it
	If we don't do this, our code will crash if age is uninitialized	
	*/
	
	len = strlen(tmpName->firstName)+strlen(tmpName->lastName)+28;
	tmpStr = (char*)malloc(sizeof(char)*len);
	
	sprintf(tmpStr, "%s %s Age: %d", tmpName->firstName, tmpName->lastName, tmpName->age);
	
	return tmpStr;
}

//We compare names by last name
int compareFunc(const void *first, const void *second){
	
	Name* tmpName1;
	Name* tmpName2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Name*)first;
	tmpName2 = (Name*)second;
	
	return strcmp((char*)tmpName1->lastName, (char*)tmpName2->lastName);
}

//We need to free the first and last names, and then the Name struct itself
void deleteFunc(void *toBeDeleted){
	
	Name* tmpName;
	
	if (toBeDeleted == NULL){
		return;
	}
	
	tmpName = (Name*)toBeDeleted;
	
	free(tmpName->firstName);
	free(tmpName->lastName);
	free(tmpName);
}

int main(void){
	xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile("quad01.svg", NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    //print_element_names(root_element);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

}









