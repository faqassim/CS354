///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      N/A
// Semester:         CS 354 Lecture 01 Fall 2022
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
#include <string.h>

// Structure that represents a magic square
typedef struct {
	int size;           // dimension of the square
	int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {

	//variable to store scanned value 
	int scanned;

	
	printf("Please enter the size of the magic square\n");
	scanf("%d", &scanned);

	//if statements to check if number input is odd and more than 3
	if (scanned < 3) { 
		printf("Size must be greater than 3!\n"); 
		exit(1);
	} 
	if (scanned % 2 == 0) { 
		printf("Size must be odd!\n"); 
		exit(1);
	}
	return scanned;   
} 

/* Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
	//block of initializations
	//setting row and column indices to middle row right-most column
	int rowIndex = ((n-1) / 2);
	int colIndex = n - 1;
	int current = 1;

	//allocating magicSquare on heap to allow usage outside function
	MagicSquare *magicSquare = malloc(sizeof(MagicSquare));
	magicSquare->size = n;

	//allocation/initialization of 2D array
	int** m = malloc(sizeof(int*) * n);
	magicSquare->magic_square = m;
	for(int i = 0; i < n; i++) {
		*(m + i) = malloc(sizeof(int) * n);

	}

	//setting one to (n/2,n)
	*(*(m + rowIndex) + colIndex) = current;

	//incrementing indices and current value
	rowIndex++; 
	colIndex++; 
	current++;

	//while current value < n^2
	while(current <= (n*n)) {

		//if both indices < n and element at said indices is empty
		//then set position to current
		if (rowIndex < n && colIndex < n && *(*(m + rowIndex) + colIndex) == 0) {
			*(*(m + rowIndex) + colIndex) = current;
			current++;
			rowIndex++;
			colIndex++;
			continue;	
		}	
		//if both indices are out of bounds
		//decrement colIndex twice to go to the left of original position
		//decrement rowIndex to go to a row that's in bounds
		if (rowIndex >= n && colIndex >= n) {
			colIndex -= 2;
			rowIndex--;
			*(*(m + rowIndex) + colIndex) = current;
			rowIndex++;
			colIndex++;
			current++;
			continue;

		}
		//if only the column is out of bounds
		if (colIndex >= n) {

			//and m[row][0] isn't empty
			if (*(*(m + rowIndex) + 0) != 0) {

				//then put current to the left of the previous element
				colIndex -= 2;

				*(*(m + rowIndex) + colIndex) = current;
				rowIndex++; 
				colIndex++;
				current++;
			} 

			//if not then just wrap around the matrix's columns and put current there
			colIndex = 0;

			*(*(m + rowIndex) + colIndex) = current;

			rowIndex++;
			colIndex++;
			current++;
			continue;
		}
		//if only the row is out of bounds
		if (rowIndex >= n) {

			//and m[row][0] isn't empty
			if (*(*(m + 0) + colIndex) != 0) {
				//put current to the left of the previous element 
				colIndex -= 2;
				rowIndex--;
				
				*(*(m + rowIndex) + colIndex) = current;
				rowIndex++;
				colIndex++;
				continue;
			}

			//if not then wrap around the matrix's 
			rowIndex = 0;

			*(*(m + rowIndex) + colIndex) = current;
			rowIndex++; 
			colIndex++;
			current++;
			continue;
		}
		//if m[row][column] isn't empty
		if (*(*(m + rowIndex) + colIndex) != 0) {

			//then put current to the left of previous
			rowIndex--;
			colIndex -= 2;

			*(*(m + rowIndex) + colIndex) = current;
			colIndex++;
			rowIndex++;
			current++;
			continue;
		}
	}
	return magicSquare;    
} 

/* Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *ofp = fopen(filename, "w");
	fprintf(ofp,"%d", magic_square->size);
	for(int i = 0; i < magic_square->size; i++) {
		fprintf(ofp,"\n");
		for(int j = 0; j < magic_square->size; j++) { 
			if(j == (magic_square->size - 1)) {

				fprintf(ofp,"%i",*(*(magic_square->magic_square + i) + j));
			} else {
				fprintf(ofp,"%i,",*(*(magic_square->magic_square + i) + j));
			}
		}
	}
	fclose(ofp);
}

/* Generates a magic square of the user specified size and
 * output the quare to the output filename
 *
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
	//Check input arguments to get output filename
	if (argc != 2) {
		printf("Usage: ./myMagicSquare <output_filename> \n");
		exit(1);
	}
	//Get magic square's size from user

	int size; 
	size = getSize();

	//Generate the magic square
	MagicSquare *magicSquare; 
	magicSquare =  generateMagicSquare(size);
	//Output the magic square
	fileOutputMagicSquare(magicSquare, argv[1]);

	//free dynamically allocated memory
	for(int i = 0; i < magicSquare->size; i++) {
		free(*(magicSquare->magic_square + i));
	}
	//free struct
	free(magicSquare->magic_square);
	free(magicSquare);
	return 0;
} 


//     F22 deppeler myMagicSquare.c      
