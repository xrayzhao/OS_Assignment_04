//Created by Ray Zhao (170152740) and Colin Faught (170456730)
//https://github.com/xrayzhao/OS_Assignment_04

#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5 //number of customers in sample4_in.txt file
#define NUMBER_OF_RESOURCES 4 //number of resources type for customer

//used in Run command

int available[NUMBER_OF_RESOURCES]; /* the available amount of each resource */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /*the maximum demand of each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the amount currently allocated to each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the remaining need of each customer */
int r;
int toBeReleased;


int get_max(char *fileName); //read the txt file and enroll elements into maximum and need array
int request_resources(int customer_num, int request[]); //request command is performed in here
void release_resources(int customer_num, int release[]); //release command is performed in here
int initial_safe_check(int customer_num, int available[], int request[]); //use to first check the safe state of the system before run calculation
int safe_state(int available[]); //checking the system is in safe state
void runner(void *thread); //used to run the program with safe algorithm
void *threadRunner(void *thread);
int sequence[NUMBER_OF_CUSTOMERS];

typedef struct thread 
{
    int customer;
    int id;
} Thread;


int main(int argc, char *argv[]) {

    char *fileName = "sample4_in.txt"; //File in
    pthread_t tid;

    //check for command line info
    if (argc < 2) {
        fprintf(stderr, "Error: available array info is missing\n");
        return -1;
    } else {
        printf("Number of Customers: %d\n", NUMBER_OF_CUSTOMERS);
        printf("Currently Available resources: ");
        //add command line info to available array
        for(int i = 1; i < NUMBER_OF_CUSTOMERS; i++){
            available[i-1] = atoi(argv[i]);
            printf("%d ",available[i-1]);
        }
        printf("\n");
    }

    printf("Maximum resources from file:\n");
    get_max(fileName); //read sample4_in.txt


    while (1) //Run the loop to ask command until an invalid command entered
    {
        char command[10]; // input array+
        char c;
        int i = 0;
        
        printf("Enter Command: ");
        //adding input command into input array
        while ((c = getchar()) != '\n') {
            command[i] = c;
            i++;
        }

        // if command begin will "RQ", Request function performs
        if (command[0] == 'R' && command[1] == 'Q') {
            int customer_num;
            int require[NUMBER_OF_RESOURCES]; //require number of resource for each type
            char *separator = (command + 3); //separate customer number and resource info from command

            sscanf(&separator[0], "%d", &customer_num); //Get customer number
            separator += 2;

            //get resource info of each type
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                sscanf(separator, "%d", &require[i]);
                separator += 2;
            }

            /*call request_resources function, print result*/
            if (request_resources(customer_num, require) == 0) {
                printf("Request is satisfied.\n");

            } else {
                fprintf(stderr, "Unsafe request! denied.\n");
            }
        }

        // if command begin will "RL", Release function performs
        else if ((command[0] == 'R' && command[1] == 'L')){
            int customer_num;
            int release[NUMBER_OF_RESOURCES]; //release number of resource for each type
            char *separator = (command + 3);

            sscanf(&separator[0], "%d", &customer_num); //Get customer number
            separator += 2;

            //get resource info of each type
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                sscanf(separator, "%d", &release[i]);
                separator += 2;
            }

            /*call release_resources function, print result*/
            release_resources(customer_num, release);

        }
        // if command begins with *, output the state of availabe, maximum, allocation, and need
        else if (command[0] == '*'){
            
            int row, col;

            //output available
            printf("Available Array State\n");
            for (col = 0; col < NUMBER_OF_RESOURCES; col++){
                printf("%d ", available[col]);
            }
            //output max
            printf("\n\nMaximum Array State\n");;
            for (row = 0; row < NUMBER_OF_CUSTOMERS; row++){
                for (col = 0; col < NUMBER_OF_RESOURCES; col++){
                    printf("%d ", maximum[row][col]);
                }
                printf("\n");
            }
            //output allocation
            printf("\nAllocation Array State\n");
            for (row = 0; row < NUMBER_OF_CUSTOMERS; row++){
                for (col = 0; col < NUMBER_OF_RESOURCES; col++){
                    printf("%d ", allocation[row][col]);
                }
                printf("\n");
            }
            //output need
            printf("\nNeed Array State\n");
            for (row = 0; row < NUMBER_OF_CUSTOMERS; row++){
                for (col = 0; col < NUMBER_OF_RESOURCES; col++){
                    printf("%d ", need[row][col]);
                }
                printf("\n");
            }
        }

        else if ((command[0] == 'R' && command[1] == 'u' && command[2] == 'n' )){

            //check the safe_state of available
            int state = safe_state(available);
            if (state == -1){
                printf("\nThe current state is NOT SAFE\n");
            }else

            //if Safe proceed to next step
            {
                printf("\nThe current state is SAFE\n");
                
                // Print the safe sequence of threads
                printf("The safe sequence is: <");

                for (int x = 0; x < NUMBER_OF_CUSTOMERS; x++) {
                    
                    //output the safe sequence
                    if(x<(NUMBER_OF_CUSTOMERS-1))
                    {
                        printf("%d ", sequence[x]);
                    }else
                    {
                        printf("%d", sequence[x]);
                    }
                }
                printf(">\n");

                //iterate through safe sequence
                for (r = 0; r < NUMBER_OF_CUSTOMERS; r++) {
                    
                    Thread *thread;

                    int request[NUMBER_OF_RESOURCES];
                    int release[NUMBER_OF_RESOURCES];

                    //uncomment to test sequences
                        //printf("sequences:");
                        //for (int c = 0; c < NUMBER_OF_CUSTOMERS; c++) {
                        //    printf(" %d", sequence[c]);
                        // }

                    printf("\n\n-->Customer/Thread %d", sequence[r]);
                    
                    //output allocated
                    printf("\n      Allocated resources:   ");
                    for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
                        printf(" %d", allocation[sequence[r]][c]);
                    }
                    
                    //output need
                    printf("\n      Needed:   ");
                    for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
                        printf(" %d", need[sequence[r]][c]);
                    }

                    //output avaiable
                    printf("\n      Available:  ");
                    for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
                        printf(" %d", available[c]);
                    }
                    printf("\n");

                    pthread_t tid;

                    //create thread
				    pthread_create(&tid, NULL, &threadRunner, thread + r);

                    //join the threads
                    pthread_join(tid, NULL);

                    
                                    //release_resources(sequence[r],allocation[sequence[r]]);
                    
                    //now that the threads have run, output the new avaiable resources
                    printf("      New Available:  ");
                    for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
                        printf(" %d", available[c]);
                    }
                    printf("\n");
                }

            }

        }
            else {

            return 0;
        }
    }

}

int get_max(char *fileName) {

    FILE *in = fopen(fileName, "r");
    if (!in) {
        printf("Error in opening input file...exiting with error code -1\n");
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
    int i = 0;
    resources = strtok(fileContent, "\r\n");
    while (resources != NULL) {
        lines[i] = malloc(sizeof(resources) * sizeof(char));
        strcpy(lines[i], resources);
        printf("%s \n", lines[i]);
        i++;
        resources = strtok(NULL, "\r\n");
    }

    for (int k = 0; k < NUMBER_OF_CUSTOMERS; k++) {
        char *token = NULL;
        token = strtok(lines[k], ","); // get tokens split by ","
        int j = 0;
        while (token != NULL) {
            maximum[k][j] = atoi(token); //add elements in maximum array
            need[k][j] = atoi(token);    //add elements in need array
            j++;
            token = strtok(NULL, ",");
        }
    }

    return 1;
}

int request_resources(int customer_num, int request[]) {

    int state = initial_safe_check(customer_num, available, request); //check the safe state/valid for a request

    //if the system is unsafe by a invalid request, return -1 to deny
    if(state == -1){
        return -1; //request_recourse denied

    } else {

        // performing logical bank's algorithm calculation for request
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] -= request[i]; //available  - request = resource for each type left
            allocation[customer_num][i] += request[i]; //allocate request add on to customer's allocation matrix
            need[customer_num][i] -= request[i]; //get resource a customer still needed
        }
    }


    int after_state = safe_state(available); //check the safe state after perform request

    //if the system is unsafe (potential a deadlock will occur), return back to previous step
    if (after_state == -1) {

        //reverse the calculations
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        return -1; //request_recourse denied

    } else {
        return 0; //request_recourse success
    }

}

void release_resources(int customer_num, int release[])
{
    for (int i=0;i<NUMBER_OF_RESOURCES;i++)
    {
        //performing logical bank's algorithm calculation for release
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];

        //if release more resource exceed customer needs, release_resources denied, return back to previous step
        if (allocation[customer_num][i] < 0)
        {
            fprintf(stderr,"Invalid release! denied.\n");

            //reverse the calculations
            for (int j=0;j<i;j++)
            {
                available[i] -= release[i];
                allocation[customer_num][i] += release[i];
                need[customer_num][i] -= release[i];
            }
        }
    }
}

int initial_safe_check(int customer_num, int available[], int request[]) {

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {

        // if request is greater than need, OR request is greater than available, the state unsafe
        //printf("Request: %d, need: %d \n", request[i], need[customer_num][i]);
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            return -1; // request denied
        }
    }

    return 1;// request accepted

}

int safe_state(int available[]){

    int work[NUMBER_OF_RESOURCES]; //request worked
    int finish[NUMBER_OF_CUSTOMERS]; //contains a list of state results
    int count = -1;
    
    //reset sequence
    for (int i = 0; i<NUMBER_OF_RESOURCES;i++){
        sequence[i] = count;
    }

    //signs available array to the work array
    for (int i = 0; i<NUMBER_OF_RESOURCES;i++){
        work[i] = available[i];
        //printf("%d ",work[i]);
    }
    //signs all index of finish to 0 (false)
    for (int i=0;i<NUMBER_OF_CUSTOMERS;i++){
        finish[i]=0;
        //printf("%d ",finish[i]);
    }
    //printf("\n");
    //run all Customers
    for (int k = 0;k < NUMBER_OF_CUSTOMERS;k++){

        //if finish index k is false, do
        if(finish[k] == 0){
            //run all Resources of a customer
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++){
                //printf("%d %d %d\n",need[k][j],work[j],j);
                //if customer resource need is greater that work, do
                if (need[k][j] > work[j]){
                    break;
                }

                //if j went over all resources
                if (j == NUMBER_OF_RESOURCES - 1){
                    finish[k] = 1; //switch finish to true
                    count++;
                    sequence[count] = k;
                    for(j = 0;j < NUMBER_OF_RESOURCES;j++){
                        work[j] += allocation[k][j];
                        //printf("%d %d %d %d\n",work[j],allocation[k][j],j,k);
                        k = -1; //return last step to check over again
                    }
                }
            }
        }
    }
    //printf("\nfinish: ");
    for (int i = 0;i < NUMBER_OF_CUSTOMERS;++i){
        //printf("%d ",finish[i]);
        if (finish[i] == 0){
            return -1;
        }
    }
    return 1;
}

//adapted from Assignment 2
void *threadRunner(void *thread)
{

	printf("      Thread has started\n");
    
                //printf("\n%d\n",sequence[r]);
                //request_resources(sequence[r],need[sequence[r]]);  
                //request_resources(sequence[r],request);
    //delay
    sleep(1);

	printf("      Thread has finished\n");

    //delay
	sleep(1);

	printf("      Thread is releasing resources\n");
    

    //releases resources
    release_resources(sequence[r],allocation[sequence[r]]);
                    //release_resources(sequence[r],release);

    //exit thread
	pthread_exit(0);

}

