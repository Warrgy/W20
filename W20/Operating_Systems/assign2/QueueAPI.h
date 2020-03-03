/**
    Based off of CIS2750 LinkedListAPI - created by Denis Nikitenko

    Transformation into Queue done by Cameron Fisher

    ID: 1057347
    Name: Cameron Fisher
    Date: Feb 24

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct queueNode{
    void* data;
    struct queueNode* next;
    struct queueNode* prev;
} Node;

typedef struct queueHead {
    Node* head;
    Node* tail;
    int length;
    void (*deleteData)(void* toBeDeleted);
    int (*compare)(const void* first,const void* second);
    char* (*printData)(void* toBePrinted);
} Queue;

typedef struct iterator {
    Node* current;
} QueueIterator;

//Initialize a queue, Has print function, delete function, compare function as parameters. This is for each node.
Queue* initializeQueue(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second));

//Initialize a node.
Node* initializeNode(void* data);

//Add the item to the back of the queue
void Push(Queue* queue, void* toBeAdded);

//Pop the top item off the queue
void* Pop(Queue* queue);

void insertFront(Queue* queue, void* toBeAdded);

//Returns the data from the head node, without deleting the node.
void* Peek(Queue* queue);

//Will delete the node, but not the data in that node
void deleteNode(Node* node);

//Return length of queue
int queueLength(Queue* queue);

//Clear all contents in the queue
void clearQueue(Queue* queue);

//Free the entire queue
void freeQueue(Queue* queue);

QueueIterator createIterator(Queue* queue);

void* nextElement(QueueIterator* iter);

//Get string representation of queue.
char* toString(Queue* queue);

void PushSorted(Queue* queue, void* toBeAdded);




