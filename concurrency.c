#include <stdio.h>
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define MAX_LIMIT 15
char parsedInput[2][5];


// Function for parsing input into an array of chars
int parsestring(char str[]) {
	int init_size = strlen(str);
	char delim[] = " ";

	char *ptr = strtok(str, delim);

    int i = 0;
	while(ptr != NULL)
	{
        strcpy(parsedInput[i], ptr);
		ptr = strtok(NULL, delim);
        //printf("%s ", parsedInput[i]);
        i++;
	}

	return 0; 
}

// Dining Philosophers Program

sem_t room;
sem_t chopstick[5];

void * philosopher(void *);
void eat(int);
int diningphilosophers()
{
	int i,a[5];
	pthread_t tid[5];
	
	sem_init(&room,0,4);
	
	for(i=0;i<5;i++)
		sem_init(&chopstick[i],0,1);
		
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * philosopher(void * num)
{
	int phil=*(int *)num;

	sem_wait(&room);
	printf("\nPhilosopher %d has entered room",phil);
	sem_wait(&chopstick[phil]);
	sem_wait(&chopstick[(phil+1)%5]);

	eat(phil);
	sleep(2);
	printf("\nPhilosopher %d has finished eating",phil);

	sem_post(&chopstick[(phil+1)%5]);
	sem_post(&chopstick[phil]);
	sem_post(&room);
}

void eat(int phil)
{
	printf("\nPhilosopher %d is eating",phil);
}

// Producer-Consumer Program

#define MaxItems 3 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        // Random "item" generation
        item = rand(); 
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item # %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Consume Item %d from Producer %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int producerconsumer(char* prodcount, char* concount)
{   
    pthread_t pro[*prodcount],con[*concount];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);
    int x, y;

    // Convert to int for iteration later
    sscanf(prodcount, "%d", &x);
    sscanf(concount, "%d", &y);

    // Fill array with integers for naming purposes
    int a[255] = {}; 
    for(int i = 0; i < 255; i++) {
        a[i] = i;
    }

    // Create a producer/consumer thread for each value passed
    for(int i = 0; i < x; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < y; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    // Join thread after creation 
    for(int i = 0; i < x; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < y; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

int main( ) {

    char str[MAX_LIMIT];
    char delim[] = " ";

    printf("Enter a value: ");
    fgets(str, MAX_LIMIT, stdin);

    parsestring(str);

    if(strcmp(parsedInput[0], "-p") == 0) {
        producerconsumer(parsedInput[2], parsedInput[4]);
        printf("\n");
    }
    else if(strcmp(parsedInput[0], "-d\n") == 0) {
        diningphilosophers();
        printf("\n");
    }
    else if(strcmp(parsedInput[0], "-b\n") == 0) {
        printf("Gobbity\n");
    }
    else {
        printf("Invalid input\n");
    }

    return 0;
}

