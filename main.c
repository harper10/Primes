
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
	   "   Usage: ./PrimeFinder <beginning number to test> <ending numberto test> \n"
	   "\n");
}

/**
 * Program entry point
 */
int main(int argc, char ** argv)
{
  //Initilize Variables
  uint128 start = alphaTou128(argv[1]);
  uint128 end = alphaTou128(argv[2]);
  uint128 n;
  int error;

  //Main Body
  if(argc != 3) {
    printUsage();
    return 0;
  }
  for (n = start;n <= end;n+=1)
    {
      char * n_str = u128ToString(n);
      char * str_n_threads = u128ToString(sqrtuint128(n));
      error = PrimeFinder(n_str, str_n_threads);
      free(str_n_threads);
      if (error == EXIT_FAILURE)
	{
	  return EXIT_FAILURE;
	}
    }
  return EXIT_SUCCESS;
}

int PrimeFinder(char * n_str, char * str_n_threads)
{
  //Initilize Variables
  FILE * fprime;
  char * fprime_str = "PrimeList";
  uint128 n;
  int n_threads;

  //Main Body
  fprime = fopen(fprime_str,"r+");
  if (fprime == NULL)
    {
      printf("File %s failed to open",fprime_str);
      return EXIT_FAILURE;
    }
  n = alphaTou128(n_str);

    // How many concurrent threads?
  errno = 0; // so we know if strtol fails
  n_threads = strtol(str_n_threads, NULL, 10);
  if (n_threads > 10)
    {
      n_threads = 10;
    }

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
    free(n_str);
    fclose(fprime);
    exit(1);
  }
    
  struct timeval time1;
  struct timeval time2;
  gettimeofday(&time1, NULL);
  printf("Testing if '%s' is prime with %d threads: ", n_str, n_threads);
  fflush(stdout);
  int is_prime = primalityTestParallel(n, n_threads, fprime);
  gettimeofday(&time2, NULL);
  if(is_prime) {
    printf("TRUE");
    fprintf(fprime,"\n%s",n_str);
  } else {
    printf("FALSE");
  }
  printf(", %8.4fs\n", timeDiff(time1, time2));

  fclose(fprime);
  free(n_str);
  return EXIT_SUCCESS;
}

