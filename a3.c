#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int get_max(char *fileName);
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);


int main()
{
    char *fileName = "/Users/rayzhao/VSCode/OS_Assignment_04/sample4_in.txt"; //File in
    get_max(fileName);
}


int get_max(char *fileName){

    FILE *in = fopen(fileName, "r");
    if (!in) {
        printf("Child A: Error in opening input file...exiting with error code -1\n");
        return -1;
    }

    struct stat st;
    fstat(fileno(in), &st);
    char *fileContent = (char *) malloc(((int) st.st_size + 1) * sizeof(char));
    fileContent[0] = '\0';
    while (!feof(in)) {
        char line[100];
        if (fgets(line, 100, in) != NULL) {
            strncat(fileContent, line, strlen(line));
        }
    }
    fclose(in);

    char *resources = NULL;
    int threadCount = 0;
    char *fileCopy = (char *) malloc((strlen(fileContent) + 1) * sizeof(char));
    strcpy(fileCopy, fileContent);
    resources = strtok(fileCopy, "\r\n");
    while (resources != NULL) {
        threadCount++;
        resources = strtok(NULL, "\r\n");
    }

    char *lines[NUMBER_OF_CUSTOMERS];
    resources = NULL;
    int i = 0;
    resources = strtok(fileContent, "\r\n");
    while (resources != NULL) {
        lines[i] = malloc(sizeof(resources) * sizeof(char));
        strcpy(lines[i], resources);
        printf("%s \n",lines[i]);
        i++;
        resources = strtok(NULL, "\r\n");
    }

    for (int k = 0; k < NUMBER_OF_CUSTOMERS; k++) {
        char *token = NULL;
        token = strtok(lines[k], ","); // get tokens split by ;
        int j = 0;
        while (token != NULL) {
            maximum[k][j] = atoi(token);
            printf("%d ",maximum[k][j]);
            j++;
            token = strtok(NULL, ",");
        }
    }

    return 1;
}

