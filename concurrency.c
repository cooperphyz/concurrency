#include <stdio.h>
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define MAX_LIMIT 15

// Global array for holding parsed input with max number of arguments 5
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

#define MAXMUNCHER 5
sem_t table;
sem_t chopstick[5];

void * philosopher(void *);
void eat(int);
int diningphilosophers()
{
    // Create a 'name' int for each thread
    int a[5] = {}; 
    for(int j = 0; j < MAXMUNCHER; j++) {
        a[j] = j;
    }

	pthread_t tid[5];
	
    //Initiate semaphore thread and join thread for each 
    // in MAXMUNCHER (5). Any less than 5 breaks
	sem_init(&table,0,4);
	for(int i = 0; i < MAXMUNCHER; i++) {
		sem_init(&chopstick[i],0,1);
    }
	for(int i = 0; i < MAXMUNCHER; i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(int i = 0; i < MAXMUNCHER; i++) {
		pthread_join(tid[i],NULL);
    }
}

void * philosopher(void * num)
{
	int philos=*(int *)num;
    // Sit down and wait at table for opening for chopsticks
	sem_wait(&table);
	printf("\nPhilosopher #%d has sat down",philos);
	sem_wait(&chopstick[philos]);
	sem_wait(&chopstick[(philos+1)%MAXMUNCHER]);

    // 'Eat' when given opportunity, print and continue with semaphore unlocking and posting status
	eat(philos);
	sleep(1);
	printf("\nPhilosopher #%d has finished eating",philos);
    // Unlock semaphore by incrementing value for next philosopher
	sem_post(&chopstick[(philos+1)%MAXMUNCHER]);
	sem_post(&chopstick[philos]);
    // Successfully post semaphore return status
	sem_post(&table);
}

void eat(int philos)
{
	printf("\nPhilosopher %d is eating",philos);
}


// Producer-Consumer Program

// Maximum items a producer can produce or a consumer can consume
#define MaxItems 3 
#define Buffer 5 

sem_t empty;
sem_t full;

int in = 0;
int out = 0;
int buffer[Buffer];

pthread_mutex_t mutex;

void *producer(void *prodno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        // Random "item" generation
        item = rand(); 
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // Add item to buffer for consumer, print randomly produced item in what buffer slot
        buffer[in] = item;
        printf("Producer %d: Insert Item # %d at %d\n", *((int *)prodno),buffer[in],in);

        // Calculate next value for next position in buffer
        in = (in+1)%Buffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *conno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // Read item from buffer, print consumed item number from what producer
        int item = buffer[out];
        printf("Consumer %d: Consume Item %d from Producer %d\n",*((int *)conno),item, out);
        // Calculate next value for next position in buffer

        out = (out+1)%Buffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int producerconsumer(char* prodcount, char* concount)
{   
    pthread_t pro[*prodcount],con[*concount];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,Buffer);
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

    // Input parsing setup
    char str[MAX_LIMIT];
    char delim[] = " ";

    printf("Enter a value: ");
    fgets(str, MAX_LIMIT, stdin);

    parsestring(str);

    // Producer/Consumer Program. Reads args in indexes 2 and 4 for producer and consumer args
    if(strcmp(parsedInput[0], "-p") == 0) {
        // Invalid input checking
        if(strcmp(parsedInput[1], "-n") != 0) {
            printf("Invalid parameters, format: -p -n <# producers> -c <# consumers>\n");
            return 0;
        }
        if(strcmp(parsedInput[3], "-c") != 0) {
            printf("Invalid parameters, format: -p -n <# producers> -c <# consumers>\n");
            return 0;
        }
        producerconsumer(parsedInput[2], parsedInput[4]);
        printf("\n");
    }
    // Dining Philosophers Program
    else if(strcmp(parsedInput[0], "-d\n") == 0) {
        diningphilosophers();
        printf("\n");
    }
    // Potion Brewers Program
    else if(strcmp(parsedInput[0], "-b\n") == 0) {
        diningphilosophers();
        printf("\n");
    }
    else {
        printf("Invalid input\n");
    }

    return 0;
}

