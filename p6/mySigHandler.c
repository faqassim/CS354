////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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

//to count how many times SIGUSR1 was handled
int counter = 0;

/**
* function to handle SIGALRM
* prints out PID and current time every 4 seconds.
*/
void SIGALRM_handler() {
	time_t t1 = time(NULL);

	//checks return value of time()
	if (!t1) {
		printf("error: cannot get time\n");
		exit(1);
	}
	
	char *t2 = ctime(&t1);

	//checks return value of ctime()
	if (!t2) {
		printf("error: cannot format time\n");
	}

	printf("PID: %d CURRENT TIME: %s", getpid(), t2);
	alarm(4);
}
/**
* function to handle SIGUSR1
* prints a message out and increments counter, which counts how many times SIGUSR1 was handled
*/
void SIGUSR1_handler() {
	printf("SIGUSR1 handled and counted!\n");
	counter++;
}

/**
* function to handle SIGINT
* prints out a message with how many times SIGUSR1 was handled, then exits
*/
void SIGINT_handler() {
	printf("\nSIGINT handled.\nSIGUSR1 was handled %d times. Exiting now.\n", counter);
	exit(0);
}

/**
* main function
* sets off an alarm by using an infinite loop 
* declares and initializes signal handlers 
*/
int main () {

	alarm(4);
	printf("PID and time print every 4 seconds.\n Type Ctrl-C to end the program.\n");

	//initialization of structs

	//SIGINT struct
	struct sigaction sa;
	//setting of memory for it
	memset(&sa, 0, sizeof(sa));
	//making sa_handler point to SIGALRM_handler
	sa.sa_handler = SIGALRM_handler;

	//SIGUSR1 struct
	struct sigaction sausr1;
	//making sa_handler point to SIGUSR1_handler
	memset(&sausr1, 0, sizeof(sausr1));
	sausr1.sa_handler = SIGUSR1_handler;

	//SIGINT struct
	struct sigaction saint;
	//making sa_handler point to SIGINT_handler
	memset(&saint, 0, sizeof(saint));
	saint.sa_handler = SIGINT_handler;


	//checking return of sigaction
	if (sigaction(SIGALRM, &sa, NULL) != 0) {
		printf("error: binding SIGALRM handler\n");
		exit(1);
	}

	if (sigaction(SIGUSR1, &sausr1, NULL) != 0) {
		printf("error: binding SIGUSR1 handler\n");
		exit(1);
	}

	if (sigaction(SIGINT, &saint, NULL) != 0) {
		printf("error: binding SIGINT handler\n");
		exit(1);
	}
		
	//infinite loop
	while(1) { }
	return 0;
}
