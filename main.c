
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "main.h"

/**
 * The number of seconds difference between t1 and t2
 */
double timeDiff(struct timeval t1, struct timeval t2)
{
  double time_diff;
  double sec_offset = 0;
  double t1_sec = t1.tv_sec;
  double t2_sec = t2.tv_sec;
  double t1_usec = t1.tv_usec;
  double t2_usec = t2.tv_usec;
  if (t2_usec < t1_usec) {
    time_diff = 1e6 + t2_usec - t1_usec;
    sec_offset = -1;
  } else {
    time_diff = t2_usec - t1_usec;
  }

  time_diff = (t2_sec - t1_sec) + sec_offset + time_diff / 1e6;
  return time_diff; // in seconds
}

/**
 * Print usage
 */
void printUsage()
{
    printf("\n"
	   "   Usage: ./pa06 <number-to-test> <number-of-parallel-threads> <file-of-primes> \n"
	   "\n");
}

/**
 * Program entry point
 */
int main(int argc, char * * argv)
{
  //Initilize Variables
  FILE * fprime;
  uint128 n;
  int n_threads;

  //Main Body
  if(argc != 4) {
    printUsage();
    return 0;
  }
  fprime = fopen(argv[3],'r+');
  if (fprime == NULL)
    {
      printf("File %s failed to open", argv[3]);
      return EXIT_FAILURE;
    }
  n = alphaTou128(argv[1]);

    // How many concurrent threads?
  errno = 0; // so we know if strtol fails
  n_threads = strtol(argv[2], NULL, 10);

  // Was there an error in the input arguments?
  int error = FALSE;
  if(errno != 0 || n_threads <= 0) {
    fprintf(stderr, "2nd argument must be a valid integer >= 1, aborting.\n");
	error = TRUE;
    }

  /*
    if(n_str && strcmp(n_str, argv[1]) != 0) {
	fprintf(stderr, "1st argument must be a valid 128-bit integer: '%s' != '%s', aborting.\n", n_str, argv[1]);
	error = TRUE;
    }
  */
  if(error) {
      //free(n_str);
    fclose(fprime);
    exit(1);
  }
  /*
    free(n_str);
  */
    
  struct timeval time1;
  struct timeval time2;
  gettimeofday(&time1, NULL);
  printf("Testing if '%s' is prime with %d threads: ", argv[1], n_threads);
  fflush(stdout);
  int is_prime = primalityTestParallel(n, n_threads, fprime);
  gettimeofday(&time2, NULL);
  if(is_prime) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
  printf(", %8.4fs\n", timeDiff(time1, time2));

  fclose(fprime);
  return EXIT_SUCCESS;
}

