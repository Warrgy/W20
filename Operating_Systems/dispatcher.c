/*
dispatcher.c

Student Name : Cameron Fisher
Student ID # : 1057347

Dispatch Algorithm : ....
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100

/*
Any required standard libraries and your header files here
*/

void dispatcher(FILE *fd, int harddrive){
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
        while ( token != NULL ){
            num_exchanges += sscanf(token, "%d",&exchange_time);
            token = strtok(NULL, " ");
        }
       printf("Process %3d wants to start at %6dms and run for %6dms and has %3d hard drive exchanges\n",  process_id, start_time, run_time, num_exchanges);    
    }
}
