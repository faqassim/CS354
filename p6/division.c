////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        division.c
// Other Files:      mySigHandler.c, sendsig.c
// Semester:         CS 354 Lecture 01? Fall 2022
// Instructor:       deppeler
//
// Author:           Farris Alkassem
// Email:            alkassem@wisc.edu
// CS Login:         alkassem
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

//global variables n1, n2 are for division and counter is for counting successful operations.
int n1;
int n2;
int counter;

//method to handle division by zero, prints out error message, number of successful operations and calls exit(0) instead of crashing.
void SIGFPE_handler() {
	printf("Error: a division by 0 operation was attempted.\nTotal number of operations completed successfully: %d\nThe program will be terminated.\n", counter);
	exit(0);
}

int main () {

	//SIGFPE_handler struct
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	//setting sa_handler to point to SIGFPE_handler 
	sa.sa_handler = SIGFPE_handler;

	//checks if sigaction doesn't return 0
	if (sigaction(SIGFPE, &sa, NULL) != 0) {
		printf("error: binding SIGFPE handler\n");
		exit(1);
	}
	
	//infinite loop to do division process
	while (1) { 
		//prompting user to enter integers.
		printf("Enter first integer: ");
		//buffer cstring
		char scan[101];
		n1 = atoi(fgets(scan, 100, stdin));
		printf("Enter second integer: ");
		n2 = atoi(fgets(scan,100,stdin));

		//printing result and remainder of division
		printf("%d / %d is %d with a remainder of %d\n", n1, n2, (n1 / n2), (n1 % n2));
		//increments counter of successful divisions
		counter++;
	}
}
