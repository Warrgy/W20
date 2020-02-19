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

    unsigned int totalRunningTime;
    unsigned int totalReadyTime;
    unsigned int totalBlockedTime;
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
int compare(const void* a, const void* b) {
    const Process* one = (const Process*) a;
    const Process* two = (const Process*) b;

    return one->runTime - two->runTime;
}

//Initialize a process
Process* initializeProcess(unsigned int pid, unsigned int startTime, unsigned int runTime, unsigned int* harddriveExchanges, unsigned int numExchanges) {
    Process* process = malloc(sizeof(Process));

    process->pid = pid;
    process->startTime = startTime;
    process->runTime = runTime;
    process->harddriveExchanges = harddriveExchanges;
    process->numExchanges = numExchanges;

    process->totalReadyTime = 0;
    process->totalRunningTime = 0;
    process->totalBlockedTime = 0;

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

void incrementReadyQueue(Queue* readyQueue) {
    QueueIterator iter = createIterator(readyQueue);
    void* node = nextElement(&iter);

    while (node != NULL) {
        Process* current = (Process*) node;

        current->totalReadyTime++;

        node = nextElement(&iter);
    }
}

void printResults(Queue* finished, Process* zero) {
    printf("0 %d\n", zero->totalRunningTime);

    QueueIterator iter = createIterator(finished);
    void* node = nextElement(&iter);

    while (node != NULL) {
        Process* current = (Process*) node;

        printf("%d %d %d %d\n", current->pid, current->totalRunningTime, current->totalReadyTime, current->totalBlockedTime);

        node = nextElement(&iter);
    }
}

void dispatcher(FILE *fd, int harddrive){
    Queue *newQueue = initializeQueue(&print,&delete,&compare);
    Queue *readyQueue = initializeQueue(&print,&delete,&compare);
    Queue *blockedQueue = initializeQueue(&print,&delete,&compare);
    Queue *doneQueue = initializeQueue(&print,&delete,&compare);
    Hardware* HDD = initializeHDD(harddrive);
    Hardware* CPU = initializeCPU();
    Process* processZero = initializeProcess(0,0,0,NULL,0);
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

    int numProcesses = queueLength(newQueue);

    unsigned int realTime = 0;
    // Simulation real time. i is the time in ms.
    for (realTime = 0; queueLength(newQueue) > 0 || queueLength(readyQueue) > 0 || queueLength(blockedQueue) > 0 || CPU->curProcess != NULL; realTime++) {

        //Ignore this. This is used for debugging purposes
        if (realTime % 1000000 == 0) {
            printf("******************************************************************\n");
            char* g = toString(newQueue);
            printf("New queue: %s\n", g);
            free(g);

            char* e = toString(readyQueue);
            printf("ready queue: %s\n", e);
            free(e);

            char* done = toString(doneQueue);
            printf("done queue: %s\n", done);
            free(done);
            
            char* c = print(CPU->curProcess);
            printf("CPU: %s\n", c);
            free(c);
            printf("******************************************************************\n");
        }

        Process* topOfNew = Peek(newQueue);

        //Take top item from start queue
        if (topOfNew != NULL) {
            if (topOfNew->startTime == realTime) {
                Pop(newQueue);
                PushSorted(readyQueue, topOfNew);
            }
        }
        
        Process* topOfReady = Peek(readyQueue);
        //Increment process zero running time if no process is currently running.
        if (topOfReady == NULL && CPU->curProcess == NULL) {
            processZero->totalRunningTime++;
            continue;
        }
        
        //Used to be a placeholder for the top of the ready. For the 3rd comparison in the if else statement below.
        unsigned int nullReadyRunTime;
        if (topOfReady == NULL)
            nullReadyRunTime = 4000000;
        else 
            nullReadyRunTime = topOfReady->runTime;

        //Move top of ready queue into CPU
        if (CPU->curProcess == NULL) {
            printf("Switching top of Ready to CPU since CPU is NULL {time = %d}\n", realTime);
            Pop(readyQueue);
            CPU->curProcess = topOfReady;   
        //If the process in CPU has no more runtime needed. Move CPU process into done queue, and get the next item from the readyQueue.
        } else if (CPU->curProcess->runTime == 0) {
            printf("CPU process runtime is zero, switching... {time = %d}\n", realTime);
            Push(doneQueue, CPU->curProcess);
            Pop(readyQueue);
            CPU->curProcess = topOfReady;
        //If process in CPU has greater runtime left than process in top of ready queue, move CPU process to ready queue and pop ready queue onto CPU
        } else if (CPU->curProcess->runTime > nullReadyRunTime) {
            printf("changing CPU process and ready queue, queue > top, {time = %d}\n", realTime);
            Pop(readyQueue);
            PushSorted(readyQueue, CPU->curProcess);
            CPU->curProcess = topOfReady;
        } 

        //Check if you have serviced all the processes
        if (queueLength(doneQueue) == numProcesses) {
            break;
        }

        //increment each queue in ready queue by 1;
        incrementReadyQueue(readyQueue);

        
        if (CPU->curProcess == NULL) {
            printf("incrementing process 0\n");
            processZero->totalRunningTime++;
        } else {
            printf("incremen.\n");
            CPU->curProcess->totalRunningTime++;
            CPU->curProcess->runTime--;
            printf("finished increm\n");
        }
    }


    char* g = toString(newQueue);
    printf("New queue: %s\n", g);
    free(g);

    char* e = toString(readyQueue);
    printf("ready queue: %s\n", e);
    free(e);

    char* done = toString(doneQueue);
    printf("done queue: %s\n", done);
    free(done);
    
    char* c = print(CPU->curProcess);
    printf("CPU: %s\n", c);
    free(c);

    printResults(doneQueue, processZero);

    free(HDD);
    free(CPU);
    free(processZero);
    freeQueue(newQueue);
    freeQueue(readyQueue);
    freeQueue(blockedQueue);
    freeQueue(doneQueue);
}
