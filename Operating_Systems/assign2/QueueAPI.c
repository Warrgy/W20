
#include "QueueAPI.h"

//Initialize a queue, Has print function, delete function, compare function as parameters. This is for each node.
Queue* initializeQueue(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)) {
    assert(printFunction != NULL);
    assert(deleteFunction != NULL);
    assert(compareFunction != NULL);

    Queue* temp = malloc(sizeof(Queue));

    temp->head = NULL;
    temp->tail = NULL;

    temp->length = 0;

    temp->deleteData = deleteFunction;
    temp->printData = printFunction;
    temp->compare = compareFunction;

    return temp;
}

//Initialize a node.
Node* initializeNode(void* data) {
    Node* temp = (Node*)malloc(sizeof(Node));
	
	if (temp == NULL){
		return NULL;
	}
	
	temp->data = data;
	temp->prev = NULL;
	temp->next = NULL;
	
	return temp;
}

//Add the item to the back of the queue
void Push(Queue* queue, void* toBeAdded) {
    if (queue == NULL || toBeAdded == NULL)
        return;
    
    (queue->length)++;

    Node* newNode = initializeNode(toBeAdded);

    if (queue->head == NULL && queue->tail == NULL){
        queue->head = newNode;
        queue->tail = queue->head;
    }else{
		newNode->prev = queue->tail;
        queue->tail->next = newNode;
    	queue->tail = newNode;
    }
}

//Pop the top item off the queue
void* Pop(Queue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    
    (queue->length)--;

    Node* front = queue->head;

    if (front->next != NULL) {
        Node* second = front->next;
        queue->head = second;
        second->prev = NULL;
    } else {
        queue->head = NULL;
        queue->tail = NULL;
    }

    void* data = front->data;
    deleteNode(front);
    
    return data;
}

void insertFront(Queue* queue, void* toBeAdded){
	if (queue == NULL || toBeAdded == NULL){
		return;
	}
	
	(queue->length)++;

	Node* newNode = initializeNode(toBeAdded);
	
    if (queue->head == NULL && queue->tail == NULL){
        queue->head = newNode;
        queue->tail = queue->head;
    }else{
		newNode->next = queue->head;
        queue->head->prev = newNode;
    	queue->head = newNode;
    }
}

//Returns the data from the head node, without deleting the node.
void* Peek(Queue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    return queue->head->data;
}

//Will delete the node, but not the data in that node
void deleteNode(Node* node) {
    free(node);
}

//Return length of queue
int queueLength(Queue* queue) {
    return queue->length;
}

//Clear all contents in the queue
void clearQueue(Queue* queue) {
    if (queue == NULL || queue->head == NULL || queue->tail == NULL)
        return;

    Node* temp;

    while (queue->head != NULL){
		queue->deleteData(queue->head->data);
		temp = queue->head;
		queue->head = queue->head->next;
		deleteNode(temp);
	}
	
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
}

//Free the entire queue
void freeQueue(Queue* queue) {
    clearQueue(queue);
    free(queue);
}

//Get string representation of queue.
char* toString(Queue* queue) {
    char* string = malloc(1);
    strcpy(string, "");
    QueueIterator iter = createIterator(queue);
    void* node = nextElement(&iter);

    while (node != NULL) {
        char* cur = queue->printData(node);
        string = realloc(string, ((strlen(string) + strlen(cur)) * sizeof(char)) + 1);
        strcat(string, cur);
        free(cur);

        node = nextElement(&iter);
    }
    return string;
}

void* nextElement(QueueIterator* iter) {
    Node* temp = iter->current;

    if (temp != NULL) {
        iter->current = iter->current->next;
        return temp->data;
    } else {
        return NULL;
    }
}

QueueIterator createIterator(Queue* queue) {
    QueueIterator iter;

    iter.current = queue->head;

    return iter;
}

void PushSorted(Queue* queue, void* toBeAdded) {
    if (queue == NULL || toBeAdded == NULL){
		return;
	}

	if (queue->head == NULL){
		Push(queue, toBeAdded);
		return;
	}
	
	if (queue->compare(toBeAdded, queue->head->data) <= 0){
		insertFront(queue, toBeAdded);
		return;
	}
	
	if (queue->compare(toBeAdded, queue->tail->data) > 0){
		Push(queue, toBeAdded);
		return;
	}
	
	Node* currNode = queue->head;
	
	while (currNode != NULL){
		if (queue->compare(toBeAdded, currNode->data) <= 0){
			Node* newNode = initializeNode(toBeAdded);
			newNode->next = currNode;
			newNode->prev = currNode->prev;
			currNode->prev->next = newNode;
			currNode->prev = newNode;
			(queue->length)++;

			return;
		}
	
		currNode = currNode->next;
	}
	
}