/*
dispatcher.c

Student Name : Cameron Fisher
Student ID # : 1057347

Dispatch Algorithm : Shortest Remaining time CPU scheduling
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100

/*
Any required standard libraries and your header files here
*/
#include "QueueAPI.h"

typedef struct processDetails {
    unsigned int pid;
    unsigned int startTime;
    unsigned int runTime;
    unsigned int* harddriveExchanges;
    unsigned int numExchanges;
} Process;

typedef struct hardwareResource {
    int time;
    Process* curProcess;
} Hardware;

//Returns the string for the Hard drive exchanges required.
char* getExchangesString(unsigned int* exchanges, unsigned int num) {
    if (exchanges == NULL)
        return NULL;
    char* string = malloc(2);
    strcpy(string, " ");

    for (int i = 0; i < num; i++) {
        char* cur = malloc(6);
        string = realloc(string, strlen(string) + 6 + 1);

        sprintf(cur, "%4d ", exchanges[i]);

        strcat(string, cur);
        free(cur);
    }

    return string;
}

//Funtion to print a Process
char* print(void* q) {
    if (q == NULL)
        return NULL;
    Process* process = (Process*) q;

    char* string = malloc(40 * sizeof(char) + 20 + 1);

    sprintf(string, "Process:\n\tpid: %3d\n\tstartTime: %6d\n\trunTime: %6d\n\tHD: ", process->pid, process->startTime, process->runTime);

    char* hdd = getExchangesString(process->harddriveExchanges, process->numExchanges);
    if (hdd != NULL) {
        string = realloc(string, strlen(string) + strlen(hdd) + 1);
        strcat(string,hdd);
        free(hdd);
    }
    string = realloc(string, strlen(string) + 3);
    strcat(string, "\n\n");

    return string;
}
//Will delete a Process
void delete(void* f){
    Process* process = (Process*) f;
    free(process->harddriveExchanges);
    free(process);
}
int compare(){return 0;}

//Initialize a process
Process* initializeProcess(unsigned int pid, unsigned int startTime, unsigned int runTime, unsigned int* harddriveExchanges, unsigned int numExchanges) {
    Process* process = malloc(sizeof(Process));

    process->pid = pid;
    process->startTime = startTime;
    process->runTime = runTime;
    process->harddriveExchanges = harddriveExchanges;
    process->numExchanges = numExchanges;

    return process;
}

//Initialize a HDD
Hardware* initializeHDD(int time) {
    Hardware* HDD = malloc(sizeof(Hardware));
    
    HDD->time = time;
    HDD->curProcess = NULL;

    return HDD;
}

//Initialize a CPU
Hardware* initializeCPU() {
    Hardware* CPU = malloc(sizeof(Hardware));

    CPU->curProcess=NULL;
    CPU->time = 0;

    return CPU;
}

void dispatcher(FILE *fd, int harddrive){
    Queue *newQueue = initializeQueue(&print,&delete,&compare);
    Queue *readyQueue = initializeQueue(&print,&delete,&compare);
    Queue *blockedQueue = initializeQueue(&print,&delete,&compare);
    Hardware* HDD = initializeHDD(harddrive);
    Hardware* CPU = initializeCPU();
    /*
        Your code here.
        You may edit the following code
    */


    char line_buffer[MAX_LINE_LENGTH];
    int start_time, run_time, process_id, num_exchanges, exchange_time;
    char *token;
    
    //Process simulation input line by line
    while (fgets(line_buffer, MAX_LINE_LENGTH, fd) != NULL && line_buffer[0] != '\n'){
        token = strtok(line_buffer, " ");
        sscanf(token, "%d",&start_time);
       
        token = strtok(NULL, " ");
        sscanf(token, "%d",&process_id);
        
        token = strtok(NULL, " ");
        sscanf(token, "%d",&run_time);
        
        num_exchanges = 0;
        token = strtok(NULL, " ");
        unsigned int* hardDriveRequests = malloc(1);
        for (int i = 1; token != NULL; i++){
            num_exchanges += sscanf(token, "%d",&exchange_time);
            hardDriveRequests = realloc(hardDriveRequests, sizeof(unsigned int) * i);
            hardDriveRequests[i - 1] = exchange_time;
            token = strtok(NULL, " ");
        }
        printf("Process %3d wants to start at %6dms and run for %6dms and has %3d hard drive exchanges\n",  process_id, start_time, run_time, num_exchanges); 

        Process* process = initializeProcess((unsigned int)process_id, (unsigned int)start_time, (unsigned int)run_time, hardDriveRequests, num_exchanges);
        Push(newQueue, process);

    }

    // int realTime = 0;
    // // Simulation real time. i is the time in ms.
    // for (realTime = 0; queueLength(newQueue) > 0 || queueLength(readyQueue) > 0 || queueLength(blockedQueue) > 0; realTime++) {
    //     Process* topOfNew = Peek(newQueue);

    //     //Take top item from start queue
    //     if (topOfNew != NULL)
    //         if (topOfNew->startTime >= realTime) {
    //             Pop(newQueue);
    //             Push(readyQueue, topOfNew);
    //         }

        

    //     Process* topofReady = Peek(readyQueue);
    //     if (topofReady != NULL)
    //         if (topofReady->)
        
        
        
    //     printf("test\n");
    // }


    char* g = toString(newQueue);
    printf("%s", g);
    free(g);

    printf("Peek = %p\n", Peek(newQueue));

    free(HDD);
    free(CPU);
    freeQueue(newQueue);
    freeQueue(readyQueue);
    freeQueue(blockedQueue);
}
