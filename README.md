# CP386 Assignment-04

## Motivation
Completed to fulfill assignment requirements in Operating Systems university course. 


## Installation 

To use the project, please follow the below steps:

Step 1.
    Clone the github repository

    git clone https://github.com/xrayzhao/OS_Assignment_04
    
Step 2.
    Open the project in preffered IDE. If you are running on Windows, using a VM is recommended. 
[VirtualBox Website](https://www.virtualbox.org/),
[OSC10e download](https://www.os-book.com/OS10/VM/OSC10e.ova)

Step 3.
    Compile using gcc:
   
    
    gcc 170152740_170456730_a04.c -pthread
    

Step 4.
    Run the code with input
    
    ./a.out # # # #


    replace the # with the Max Resource amount you wish to assign to each Customer/Thread
    

## Screenshots 

![Screenshot1](/image/request.png "Test Request with *")

![Screenshot2](/image/release_1.png "Test Release part 1 with * ")

![Screenshot3](/image/release_2.png "Test Release part 2 with * ")

![Screenshot4](/image/Run_1.png "Test Run command Part 1 ")

![Screenshot5](/image/Run_2.png "Test Run command Part 2 ")


## Individual contribution 
    
    Ray Zhao:
    -get_max function
    -request_resource function
    -release_resource function
    -initial_safe_check function
    -safe_state function
    -LQ RQ commands
    -detailed comments
    -README File tests

    Colin Faught:
    -thread_runner function
    -adapted safe_state function 
    -created * command
    -created Run command
    -detailed comments
    -output formatting
    -README File text

## Tests 
    ./Assignment_a04 10 5 7 8

    -Tests for Request
        RQ 0 3 1 2 1 (Request satisfied)
        RQ 1 1 1 1 1 (Request satisfied)
        RQ 0 1 0 0 0 (Request satisfied)
        RQ 3 0 1 1 1 (Request satisfied)
        RQ 4 1 1 1 1 (Request satisfied)
        RQ 4 2 1 1 2 (Request denied)
        RQ 2 0 0 0 1 (Request satisfied)
        RQ 2 1 2 2 0 (Request denied)
        RQ 1 9 3 3 5 (Request denied)
        *            (Print result)

    -Tests for Release
        RL 0 4 0 0 0 (Release satisfied)
        RL 0 1 1 1 1 (Release denied)
        RL 1 1 1 1 1 (Release satisfied)
        RL 1 0 2 1 1 (Release denied)
        RL 4 1 1 1 1 (Release satisfied)
        RL 0 0 1 2 1 (Release satisfied)
        RL 2 0 0 0 1 (Release satisfied)
        RL 3 0 1 1 0 (Release satisfied)
        RL 3 0 1 1 1 (Release denied)
        RL 3 0 0 0 1 (Release satisfied)
        *            (Print result)

    -Test for Run
        See screenshots for detailed result.

## Code Example 
```c
int available[NUMBER_OF_RESOURCES]; /* the available amount of each resource */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /*the maximum demand of each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the amount currently allocated to each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; /* the remaining need of each customer */

int get_max(char *fileName); //read the txt file and enroll elements into maximum and need array
int request_resources(int customer_num, int request[]); //request command is performed in here
void release_resources(int customer_num, int release[]); //release command is performed in here
int initial_safe_check(int customer_num, int available[], int request[]); //use to first check the safe state of the system before run calculation
int safe_state(int available[]); //checking the system is in safe state
void runner(void *thread); //used to run the program with safe algorithm
void *threadRunner(void *thread);

```


## Authors 
Ray Zhao @xrayzhao

Colin Faught @f-colin

## Credits 
Operating System Concepts Ed.10


## License (MIT)
Copyright (c) 2020 Ray Zhao, Colin Faught

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.