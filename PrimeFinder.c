#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>



uint128 alphaTou128(const char * str)
{
    uint128 ret = 0;
    while(*str >= '0' && *str <= '9') {
	ret *= 10; // "left-shift" a base-ten number
	ret += (*str - '0'); // add in the units
	++str;
    }
    return ret;
}

int primalityTestParallel(uint128 value, int n_threads, FILE * fprime)
{
  //Initilize Variables
  pthread_t * th = malloc(sizeof(pthread_t)*n_threads);
  pthread_attr_t * attr = malloc(sizeof(pthread_attr_t)*n_threads);
  objData ** obj = malloc(sizeof(objData)*n_threads);
  int i;
  uint128 start = 0;
  uint128 interval =  sqrtuint128(value)/n_threads;
  uint128 end;

  //Main Body
  for (i = 0;i < n_threads;i++)
    {
      end = start + interval;
      pthread_attr_init(&attr[i]);
      obj[i] = objCreate(value,start,end,fprime);
      start = end;
      pthread_create(&th[i],&attr[i],&testPrime,obj[i]);
    }
  for (i = 0;i < n_threads;i++)
    {
      pthread_join(th[i],NULL);
      if (obj[i]->isPrime == FALSE)
	{
	  free(th);
	  free(attr);
	  deleteObjArray(obj,n_threads);
	  return FALSE;
	}
    }
  free(th);
  free(attr);
  deleteObjArray(obj,n_threads);
  return TRUE;
}

void deleteObjArray(objData ** obj, int n_threads)
{
  int i;
  for (i = 0;i<n_threads;i++)
    {
      free(obj[i]);
    }
  free(obj);
}

objData * objCreate(uint128 value, uint128 start, uint128 end, FILE * fprime)
{
  //Initilize Variables
  objData * obj = malloc(sizeof(objData));

  //Main Body
  obj->value = value;
  if (start < 2)
    {
      start = 2;
    }
  obj->start = start;
  obj->end = end;
  obj->isPrime = TRUE;
  obj->primes = fprime;

  return obj;
}

uint128 sqrtuint128(uint128 value)
{
  //Initilize Variables
  uint128 root;
  //Main Body
  for (root = 0;root*root < value;root++);
  if (root*root != value)
    {
      root--;
    }
  return root;
}  

void * testPrime(void * obj)
{
  //Initilize Variables
  int i;
  objData * data = obj;

  //Main Body
  if (data->value == 2)
    {
      data->isPrime = TRUE;
      return data;
    }
  if (data->value % 2 == 0)
    {
      data->isPrime = FALSE;
      return data;
    }
  if (data->start % 2 == 0)
    {
      data->start++;//Make sure that start value is odd
    }
  for (i = data->start;i <= data->end;i+=2)
    {
      if (data->value % i == 0)
	{
	  data->isPrime = FALSE;
	  return data;
	}
    }
  return data;
}
