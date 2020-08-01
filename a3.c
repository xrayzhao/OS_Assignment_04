#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5 //number of customers in sample4_in.txt file
#define NUMBER_OF_RESOURCES 4 //number of resources type for customer


int available[NUMBER_OF_RESOURCES]; /* the available amount of each resource */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /*the maximum demand of each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the amount currently allocated to each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the remaining need of each customer */

int get_max(char *fileName); //read the txt file and enroll elements into maximum and need array
int request_resources(int customer_num, int request[]); //request command is performed in here
void release_resources(int customer_num, int release[]); //release command is performed in here
int initial_safe_check(int customer_num, int available[], int request[]); //use to first check the safe state of the system before run calculation
int safe_state(int available[]); //checking the system is in safe state


int main(int argc, char *argv[]) {

    char *fileName = "/Users/rayzhao/CLionProjects/170152740_a04/sample4_in.txt"; //File in

    //check for command line info
    if (argc < 2) {
        fprintf(stderr, "Error: available array info is missing\n");
        return -1;
    } else {
        //add command line info to available array
        for(int i = 1; i < NUMBER_OF_CUSTOMERS; i++){
            available[i-1] = atoi(argv[i]);
        }
    }

    get_max(fileName); //read sample4_in.txt

    while (1) //Run the loop to ask command until an invalid command entered
    {
        char command[10]; // input array
        char c;
        int i = 0;

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

                    //testing for now
                    printf("%d Need: ", customer_num);
                    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                        printf("%d ", need[customer_num][i]);
                    }
                    printf("\n");
                    printf("%d Available: ", customer_num);
                    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                    printf("%d ", available[i]);
                }
                printf("\n");
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

                //for testing
                printf("%d Need: ", customer_num);
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                    printf("%d ", need[customer_num][i]);
                }
                printf("\n");

                printf("%d Available: ", customer_num);
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                printf("%d ", available[i]);
                }
                printf("\n");
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


