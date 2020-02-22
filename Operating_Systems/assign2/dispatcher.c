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
#include <stdbool.h>

typedef struct processDetails {
    unsigned int pid;
    unsigned int startTime;
    int runTime;
    unsigned int* harddriveExchanges;
    unsigned int numExchanges;

    unsigned int totalRunningTime;
    unsigned int totalReadyTime;
    unsigned int totalBlockedTime;

    unsigned int insideHDDTime;
    unsigned int exchangesSoFar;
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
//How to compare processes (Used to determine shorted time remaining)
int compare(const void* a, const void* b) {
    const Process* one = (const Process*) a;
    const Process* two = (const Process*) b;

    if (one->runTime - two->runTime == 0)
        return one->startTime - two->startTime;
    else
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

    process->insideHDDTime = 0;
    process->exchangesSoFar = 0;
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

//Used to increment the totalReadyTime or totalBlockedTime depending on the type of the queue
void incrementQueue(Queue* queue, char type) {
    QueueIterator iter = createIterator(queue);
    void* node = nextElement(&iter);

    while (node != NULL) {
        Process* current = (Process*) node;
        if (type == 'r')
            current->totalReadyTime++;
        else if (type == 'b')
            current->totalBlockedTime++;

        node = nextElement(&iter);
    }
}

//Will print out the required results once all processes have been serviced
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

//Will check if the process needs some action from the current time.
bool checkForHDDRequest(Process* p, unsigned int time) {
    if (p == NULL)
        return false;
    unsigned int* temp = p->harddriveExchanges;
    for (int i = 0; i < p->numExchanges; i++) {
        if (*temp == time) {
            return true;
        }
        temp++;
    }
    return false;
}

//Main function to run shortest remaining time first algorithm
void dispatcher(FILE *fd, int harddrive){
    Queue *newQueue = initializeQueue(&print,&delete,&compare);
    Queue *readyQueue = initializeQueue(&print,&delete,&compare);
    Queue *blockedQueue = initializeQueue(&print,&delete,&compare);
    Queue *doneQueue = initializeQueue(&print,&delete,&compare);
    Hardware* HDD = initializeHDD(harddrive);
    Hardware* CPU = initializeCPU();
    Process* processZero = initializeProcess(0,0,0,NULL,0);


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
        // printf("Process %3d wants to start at %6dms and run for %6dms and has %3d hard drive exchanges\n",  process_id, start_time, run_time, num_exchanges); 

        Process* process = initializeProcess((unsigned int)process_id, (unsigned int)start_time, (unsigned int)run_time, hardDriveRequests, num_exchanges);
        Push(newQueue, process);

    }

    int numProcesses = queueLength(newQueue);

    unsigned int realTime = 0;
    // Simulation real time. realTime is the time in ms.
    for (realTime = 0; queueLength(newQueue) > 0 || queueLength(readyQueue) > 0 || queueLength(blockedQueue) > 0 || CPU->curProcess != NULL || HDD->curProcess != NULL; realTime++) {
        
        
        Process* topOfNew = Peek(newQueue);

        //Take top item from start queue
        if (topOfNew != NULL) {
            if (topOfNew->startTime == realTime) {
                Pop(newQueue);
                PushSorted(readyQueue, topOfNew);
            }
        }
        
        Process* topOfReady = Peek(readyQueue);
        //Increment process zero running time if no process is currently running in CPU.
        if (topOfReady == NULL && CPU->curProcess == NULL) {
            processZero->totalRunningTime++;
        }

        // if (realTime % 100 == 0) {
        //     printf("************************************\n");
        //     printf("processZero = %d {time = %d}\n", processZero->totalRunningTime, realTime);
        //     char* g = toString(newQueue);
        //     printf("New queue: %s\n", g);
        //     free(g);

        //     char* e = toString(readyQueue);
        //     printf("ready queue: %s\n", e);
        //     free(e);

        //     char* done = toString(doneQueue);
        //     printf("done queue: %s\n", done);
        //     free(done);

        //     char* block = toString(blockedQueue);
        //     printf("blocked queue: %s\n", block);
        //     free(block);
            
        //     char* c = print(CPU->curProcess);
        //     printf("CPU: %s\n", c);
        //     free(c);
        //     if (CPU->curProcess != NULL)
        //         printf("numExchanges = %d, ExchangesSoFar = %d\n", CPU->curProcess->numExchanges, CPU->curProcess->exchangesSoFar);
            
        //     char* hd = print(HDD->curProcess);
        //     printf("HDD: %s\n", hd);
        //     free(hd);
        //     printf("************************************\n");
        // }

        //If absolutely nothing is happening, then just continue time.
        if (CPU->curProcess == NULL && topOfReady == NULL && HDD->curProcess == NULL && Peek(blockedQueue) == NULL) {
            continue;
        }

        //Used to be a placeholder for the top of the ready. For the 3rd comparison in the if else statement below.
        unsigned int nullReadyRunTime;
        if (topOfReady == NULL)
            nullReadyRunTime = 4000000;
        else 
            nullReadyRunTime = topOfReady->runTime;

        // if (CPU->curProcess != NULL)
        // if (CPU->curProcess->runTime == -1)
        //     printf("checking cpu if statement [numEx = %d, excSF = %d] {time = %d}\n", CPU->curProcess->numExchanges, CPU->curProcess->exchangesSoFar, realTime);
        //Move top of ready queue into CPU
        if (CPU->curProcess == NULL) {
            Pop(readyQueue);
            CPU->curProcess = topOfReady;   
        //If the process in CPU has no more runtime needed. Move CPU process into done queue, and get the next item from the readyQueue.
        } else if (CPU->curProcess->runTime <= 0 && (CPU->curProcess->numExchanges == CPU->curProcess->exchangesSoFar)) {
            // printf("MOVING CPU PROCESS TO DONE QUEUE, %d\n", CPU->curProcess->totalRunningTime);
            Push(doneQueue, CPU->curProcess);
            if (CPU->curProcess->runTime < 0) {
                CPU->curProcess->totalRunningTime--;
                processZero->totalRunningTime++;
                if (queueLength(doneQueue) == numProcesses)
                    processZero->totalRunningTime--;
            }
            Pop(readyQueue);
            CPU->curProcess = topOfReady;
        //If process in CPU has greater runtime left than process in top of ready queue, move CPU process to ready queue and pop ready queue onto CPU
        } else if (checkForHDDRequest(CPU->curProcess, CPU->curProcess->totalRunningTime)) {
            // printf("Moving from CPU to HDD {time = %d}\n", realTime);
            Push(blockedQueue, CPU->curProcess);
            CPU->curProcess = Pop(readyQueue);
        } else if (CPU->curProcess->runTime > nullReadyRunTime) {
            Pop(readyQueue);
            PushSorted(readyQueue, CPU->curProcess);
            CPU->curProcess = topOfReady;
        } 

        //If process currently getting serviced is finished, bring it to ready queue, and add the next item to the hardware.
        if (HDD->curProcess != NULL) {
            if (HDD->curProcess->insideHDDTime == HDD->time) {
                // printf("moving HDD process to ready queue {time = %d}\n", realTime);
                HDD->curProcess->insideHDDTime = 0;
                HDD->curProcess->exchangesSoFar++;
                PushSorted(readyQueue, HDD->curProcess);
                Process* topOfB = Pop(blockedQueue);
                if (topOfB == NULL)
                    HDD->curProcess = NULL;
                else 
                    HDD->curProcess = topOfB;
                continue;
            }

        } else {
            // printf("HDD is NULL -> Moving from blocked queue{len = %d} to HDD\n", queueLength(blockedQueue));
            HDD->curProcess = Pop(blockedQueue);
            // printf("PROCESS IN HDD = %p\n", HDD->curProcess);
        }

        //Check if you have serviced all the processes
        if (queueLength(doneQueue) == numProcesses) {
            break;
        }

        //increment each queue by 1;
        incrementQueue(readyQueue, 'r');
        incrementQueue(blockedQueue, 'b');

        //Increment blocked time and inside HDD time if the HDD has a process in it.
        if (HDD->curProcess != NULL) {
            HDD->curProcess->totalBlockedTime++;
            HDD->curProcess->insideHDDTime++;
        }
        
        //Increment running time and decrement runtime for process in CPU
        if (CPU->curProcess != NULL) {
            CPU->curProcess->totalRunningTime++;
            CPU->curProcess->runTime--;
        }
    }


    // char* g = toString(newQueue);
    // printf("New queue: %s\n", g);
    // free(g);

    // char* e = toString(readyQueue);
    // printf("ready queue: %s\n", e);
    // free(e);

    // char* done = toString(doneQueue);
    // printf("done queue: %s\n", done);
    // free(done);

    // char* block = toString(blockedQueue);
    // printf("blocked queue: %s\n", block);
    // free(block);
    
    // char* c = print(CPU->curProcess);
    // printf("CPU: %s\n", c);
    // free(c);
    
    // char* hd = print(HDD->curProcess);
    // printf("HDD: %s\n", hd);
    // free(hd);

    if (processZero->totalRunningTime % 100 == 99) {
        processZero->totalRunningTime++;
    }

    printResults(doneQueue, processZero);

    free(HDD);
    free(CPU);
    free(processZero);
    freeQueue(newQueue);
    freeQueue(readyQueue);
    freeQueue(blockedQueue);
    freeQueue(doneQueue);
}
