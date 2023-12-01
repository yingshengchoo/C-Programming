/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
   Copyright 2023 Vincent Choo
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"

/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start);

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size);

/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
int main(int argc, char** argv ) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;
  int RANDOM_SEED;
  /* write code to set the above values based on the input arguments
     if there is not a corresponding input argument use the
     default value in the spec. */
  NTRIALS = 10000;
  PCTGET = 50;
  PCTLARGE = 10;
  SMALL_L = 200;
  LARGE_L = 20000;
  RANDOM_SEED = time(NULL);

  if (argc > 1) NTRIALS = atoi(argv[1]);
  if (argc > 2) PCTGET = atoi(argv[2]);
  if (argc > 3) PCTLARGE = atoi(argv[3]);
  if (argc > 4) SMALL_L = atoi(argv[4]);
  if (argc > 5) LARGE_L = atoi(argv[5]);
  if (argc > 6) RANDOM_SEED = atoi(argv[6]);

  srand(RANDOM_SEED);

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  int ntrials = 0, nblocks = 0;
  clock_t start = clock();

  // perform NTRIALS mem operations

  /* Students will fill in this section according to the assignment
     specs.*/
  for (; ntrials < NTRIALS; ntrials++) {
    if (rand() % 100 < PCTGET) {
      int block_size;
      if(rand() % 100 < PCTLARGE) {
        block_size = rand() % (LARGE_L - SMALL_L) + SMALL_L;
      } else {
        block_size = rand() % SMALL_L + 1;
      }

      blocks[nblocks] = getmem(block_size);
      if (block_size > 16) block_size = 16;
        fill_mem(blocks[nblocks], block_size);
        nblocks++;
    } else {
      if (nblocks > 0) {
        int rand_block = rand() % nblocks;
        freemem(blocks[rand_block]);
      }
    }

    if(ntrials !=0 && ntrials % (NTRIALS / 10) == 0) {
      print_stats(start);
    }
   }
  print_stats(start);
  return EXIT_SUCCESS;
}

/* Define helper functions here. */

