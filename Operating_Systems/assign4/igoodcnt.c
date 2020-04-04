#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int NITER = 1000000;

int cnt = 0;

sem_t mutex;

void * Count(void * a)
{
	//Setup a semaphore wait to block other threads from accessing the resource
	sem_wait(&mutex);
	int i, tmp;
	for(i = 0; i < NITER; i++)
	{
		tmp = cnt;      /* copy the global cnt locally */
		tmp = tmp+1;    /* increment the local copy */
		cnt = tmp;      /* store the local value into the global cnt */ 
	}
	//Release and let other threads access the resource
	sem_post(&mutex);
	return NULL;
}

int main(int argc, char * argv[])
{
	//Initialize threads, and the semaphore
	pthread_t tid1, tid2;
	sem_init(&mutex, 0, 1);

	//Make sure they entered 2 arguments from the command line.
	if (argc != 2) {
		fprintf(stderr, "Please do %s {NITER value}\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Check to see if the value they entered is a valid positive integer
	int val = atoi(argv[1]);
	if (val <= 0) {
		fprintf(stderr, "Please enter a valid integer as the argument.\n");
		exit(EXIT_FAILURE);
	}
	NITER = val;

	//Display what they entered
	printf("You've entered: %d\n", val);

	printf("2*NITER is [%d]\n", 2*NITER);

	// creating Thread 1
	if(pthread_create(&tid1, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 1");
		exit(1);
	}

	// creating Thread 2
	if(pthread_create(&tid2, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 2");
		exit(1);
	}

	if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
	{
		printf("\n ERROR joining thread");
		exit(1);
	}

	if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
	{
		printf("\n ERROR joining thread");
		exit(1);
	}

	printf("\nCounter is [%d]\n", cnt);
	if (cnt == 2 * NITER) 
		printf("\nPASS\n");
	else
		printf("\nFAIL\n");

	sem_destroy(&mutex);
	pthread_exit(NULL);
}


