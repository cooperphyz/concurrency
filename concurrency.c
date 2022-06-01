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

int main( ) {

    char str[MAX_LIMIT];
    char delim[] = " ";

    printf("Enter a value: ");
    fgets(str, MAX_LIMIT, stdin);

    parsestring(str);

    if(strcmp(parsedInput[0], "-p") == 0) {
        printf("Gobbity\n");
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

