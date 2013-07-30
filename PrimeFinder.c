#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "main.h"


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

char * u128ToString(uint128 value)
{
  //INitilize Variables
  char * n_str;
  int strLen;
  int num;
  uint128 val = value;

  //Main Body
  while (val > 0)
    {
      strLen++;
      val /= 10;
    }
  n_str = malloc(sizeof(char)*strLen+1);
  n_str[strLen] = '\0';
  while (value > 0)
    {
      strLen--;
      num = value%10;
      value /= 10;
      n_str[strLen] = num + '0';
    }
  return n_str;
}

int primalityTestParallel(uint128 value, int n_threads, FILE * fprime)
{
  //Initilize Variables
  pthread_t * th = malloc(sizeof(pthread_t)*n_threads);
  pthread_attr_t * attr = malloc(sizeof(pthread_attr_t)*n_threads);
  objData ** obj = malloc(sizeof(objData)*n_threads);
  int i;
  uint128 start = 0;
  uint128 interval;
  uint128 end;
  uint128 * primelist;

  //Main Body
  primelist = createList(fprime);
  interval = findIndex(value,primelist)/n_threads;
  if(interval < 1)
    {
      interval = 1;
    }
  for (i = 0;i < n_threads;i++)
    {
      end = start + interval;
      pthread_attr_init(&attr[i]);
      obj[i] = objCreate(value,start,end,primelist);
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
	  free(primelist);
	  deleteObjArray(obj,n_threads);
	  return FALSE;
	}
    }
  free(th);
  free(attr);
  free(primelist);
  deleteObjArray(obj,n_threads);
  return TRUE;
}

int findIndex(uint128 value, uint128 * list)
{
  //Initilize Variables
  int i;
  uint128 sqrtVal = sqrtuint128(value);

  //Main Body
  for (i = 0;list[i] <= sqrtVal;i++);
  if(list[i] == sqrtVal)
    {
      i--;
    }
  return i;
}

uint128 * createList(FILE * fprime)
{
  //Initilize Variables
  uint128 * list;
  int count = 0;
  char * n = malloc(sizeof(char)*40);
  int i;

  //Main Body
  while (!feof(fprime))
    {
      count++;
      fgets(n,40,fprime);
    }
  list = malloc(sizeof(uint128)*count);
  fseek(fprime,0,0);
  for(i = 0;i < count; i++)
    {
      //fread(&n,sizeof(char),1,fprime);
      fgets(n, 40, fprime);
      list[i] = alphaTou128(n);
    }
  free(n);
  return list;
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

objData * objCreate(uint128 value, uint128 start, uint128 end, uint128 * list)
{
  //Initilize Variables
  objData * obj = malloc(sizeof(objData));

  //Main Body
  obj->value = value;
  obj->start = start;
  obj->end = end;
  obj->isPrime = TRUE;
  obj->primes = list;

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
  for (i = data->start;i <= data->end;i++)
    {
      if (data->value % data->primes[i] == 0)
	{
	  data->isPrime = FALSE;
	  return data;
	}
    }
  return data;
}
