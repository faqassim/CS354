////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        csim.c
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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for Fall 2022
//
////////////////////////////////////////////////////////////////////////////////

/*
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLES **********************************************/

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/

//counter variable used in access_data
int counter = 0;

//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.
typedef struct cache_line {                    
	char valid;
	mem_addr_t tag;
	//Add a data member as needed by your implementation for LRU tracking.
	int lru;
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;

//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache we're simulating. 
//Note: A cache is a pointer to a heap array of one or more cache sets.
cache_t cache;  

/* 
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {    		
	//initialization of S and B values	
	S = 1;
	B = 1;

	S = S << s;
	B = B << b;

	//initialization of cache
	cache = (malloc(sizeof(cache_set_t) * S));

	//check if malloc returned something 
	if (cache == NULL) {
		printf("Unable to allocate memory.\n");
		exit(1);
	}

	//initialization of lines
	for (int set = 0; set < S; set++) {

		cache[set] = (malloc(sizeof(cache_line_t) * E));
		if (cache == NULL) {
			printf("Unable to allocate memory.\n");
			exit(1);
		}
		for (int line = 0; line < E; line++) {
			cache[set][line].valid = '\0';
			cache[set][line].tag = 0;
			cache[set][line].lru = 0;
		}
	}

}


/* 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {    

	//free every element of cache         
	for (int i = 0; i < S; i++) {
		free(cache[i]); 
	}
	//free cache itself
	free(cache);
}


/* 
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cn
 */                    
void access_data(mem_addr_t addr) {

	//shift address to get tag and set values
	mem_addr_t tag = addr >> (s + b);
	int set = addr >> b;

	//use bit masking to get correct set number
	int mask = 1 << (s);
	mask--;
	set = set & mask;

	//initialization of min line
	cache_line_t  min = cache[set][0];

	//loop through contents of cache 
	for (int i = 0; i < E; i++) { 

		// if its valid and the tag matches
		if (cache[set][i].valid && cache[set][i].tag == tag) {

			//increment hit count, counter and set lru to the new counter value
			hit_cnt++;
			counter++;
			cache[set][i].lru = counter;
			return;
		}
	}
	//if you leave the loop then its a miss
	miss_cnt++;

	//loop through contents of cache
	for(int i = 0; i < E; i++) {

		//if there's an empty line, fill it and change the lru value 
		if (cache[set][i].valid == '\0') {
			cache[set][i].valid = 'v';
			cache[set][i].tag = tag;
			counter++;
			cache[set][i].lru = counter;
			return;
		}

		//look for the least recently used
		if (cache[set][i].lru < min.lru) {
			min = cache[set][i];
		}
	}
	//if you leave the loop that means the cache is full and its an eviction miss
	evict_cnt++;

	//loop through contents of cache
	for (int i = 0; i < E; i++) {

		//look for the least recently used inside the cache
		if (cache[set][i].tag == min.tag) {

			//set it to the new tag and make it the most recently used
			cache[set][i].tag = tag;
			counter++;
			cache[set][i].lru = counter;
			return;
		}
	}
	return;
}


/* 
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
	char buf[1000];  
	mem_addr_t addr = 0;
	unsigned int len = 0;
	FILE* trace_fp = fopen(trace_fn, "r"); 

	if (!trace_fp) { 
		fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
		exit(1);   
	}

	while (fgets(buf, 1000, trace_fp) != NULL) {
		if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
			sscanf(buf+3, "%llx,%u", &addr, &len);

			if (verbosity)
				printf("%c %llx,%u ", buf[1], addr, len);

			// call access_data function here depending on type of access, where M accesses twice and S and L access once
			if (buf[1] == 'M') {
				access_data(addr); 
				access_data(addr);
			} 
			if (buf[1] == 'S' || buf[1] == 'L') access_data(addr);
		}
		if (verbosity)
			printf("\n");
	}

	fclose(trace_fp);
}  


/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
	printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
	printf("Options:\n");
	printf("  -h         Print this help message.\n");
	printf("  -v         Optional verbose flag.\n");
	printf("  -s <num>   Number of s bits for set index.\n");
	printf("  -E <num>   Number of lines per set.\n");
	printf("  -b <num>   Number of b bits for block offsets.\n");
	printf("  -t <file>  Trace file.\n");
	printf("\nExamples:\n");
	printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
	printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
	exit(0);
}  


/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
	printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
	FILE* output_fp = fopen(".csim_results", "w");
	assert(output_fp);
	fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
	fclose(output_fp);
}  


/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
	char* trace_file = NULL;
	char c;

	// Parse the command line arguments: -h, -v, -s, -E, -b, -t 
	while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
		switch (c) {
			case 'b':
				b = atoi(optarg);
				break;
			case 'E':
				E = atoi(optarg);
				break;
			case 'h':
				print_usage(argv);
				exit(0);
			case 's':
				s = atoi(optarg);
				break;
			case 't':
				trace_file = optarg;
				break;
			case 'v':
				verbosity = 1;
				break;
			default:
				print_usage(argv);
				exit(1);
		}
	}

	//Make sure that all required command line args were specified.
	if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
		printf("%s: Missing required command line argument\n", argv[0]);
		print_usage(argv);
		exit(1);
	}

	//Initialize cache.
	init_cache();

	//Replay the memory access trace.
	replay_trace(trace_file);

	//Free memory allocated for cache.
	free_cache();

	//Print the statistics to a file.
	//DO NOT REMOVE: This function must be called for test_csim to work.
	print_summary(hit_cnt, miss_cnt, evict_cnt);
	return 0;   
}  

// 202209
