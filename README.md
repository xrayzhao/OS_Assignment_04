# CP386 Assignment-04

## Motivation
Completed to fulfill assignment requirements in Operating Systems university course. 


##Installation 

To use the project, please follow the below steps:
Step 1.
    Clone the github repository
    ```git clone https://github.com/xrayzhao/OS_Assignment_04
    git clone 
    ```
Step 2.
    Open the project in preffered IDE. If you are running on Windows, using a VM is recommended. 
    [VirtualBox Website](https://www.virtualbox.org/)
    [OSC10e download](https://www.os-book.com/OS10/VM/OSC10e.ova)

Step 3.
    Compile using gcc:
    ```c
    gcc 170152740_170456730_a04.c -pthread
    ```
Step 4.
    Run the code with input
    ```c
    ./a.out # # # #
    ```
    replace the # with the Max Resource you wish to allocate to each Customer/Thread
    

##Screenshots 



##Individual contribution 
    Ray Zhao:
    -get_max function
    -request_resource function
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

##Tests 



##Code Example 



##Authors 
Ray Zhao
Colin Faught

##Credits 
Operating System Concepts Ed.10


##License (MIT)
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