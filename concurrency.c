#include <stdio.h>
#include <string.h>

#define MAX_LIMIT 15
char parsedInput[2][5];

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

int main( ) {

   char str[MAX_LIMIT];
   char delim[] = " ";

   printf("Enter a value: ");
   fgets(str, MAX_LIMIT, stdin);

   parsestring(str);

   printf("%s\n", parsedInput[0]);
   printf("%s\n", parsedInput[1]);
   printf("%s\n", parsedInput[2]);
   printf("%s\n", parsedInput[3]);
   printf("%s\n", parsedInput[4]);

   return 0;
}
