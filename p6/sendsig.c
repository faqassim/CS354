////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/**
* function to send SIGINT
*
* param - pid, PID to interrupt
*/
void send_sigint(int pid) {
	int temp = 0;
	temp = kill(pid, SIGINT);

	//checking if kill() returned null
	if (temp < 0 ) {
		printf("error: unable to send SIGINT\n");
	}
}

/**
* function to send SIGINT 
* 
* param - pid, PID to send the SIGUSR1 to
*/
void send_sigusr1(int pid) {
	int temp = 0;
	temp = kill(pid, SIGUSR1);

	//checking if kill() returned null
	if (temp < 0 ) {
		printf("error: unable to send SIGUSR1\n");
	}
	exit(0);
}

/**
* main function
* checks user input if it matches requirements then calls the proper sender functions 
*/
int main(int argc, char **argv) {
	//tokenize PID
	int pid = atoi(argv[2]);

	//checks return value of atoi()
	if (pid < 0) { 
		printf("invalid PID, PID cannot be negative.\n");
	}   
	
	/**
	* checks if number of arguments  is correct
	* checks if the command inputted is correct (-i or -u)
	*/	
	if (argc == 2 || strcmp(argv[1], "-i") != 0|| strcmp(argv[1], "-u") != 0) {
		if (strcmp(argv[1], "-i") == 0) {
			send_sigint(pid);
		} else if (strcmp(argv[1], "-u") == 0) {
			send_sigusr1(pid);
		}		
	} else {
		printf("Usage: sendsig <signal type> <pid>\n");
	}
}
