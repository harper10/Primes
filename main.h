
typedef __uint128_t uint128;

#define SUCCESS 0
#define FAILURE -1

#define FALSE 0
#define TRUE 1

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

// Convert uint128 to/from strings
uint128 alphaTou128(const char * str);

typedef struct 
{
  uint128 * primes;
  uint128 value;
  uint128 start;
  uint128 end;
  int isPrime;
}objData;

// Return TRUE or FALSE
int primalityTestParallel(uint128 value, int n_threads);
uint128 sqrtuint128(uint128 value);
void * testPrime(void * obj);
objData * objCreate(uint128 value, uint128 start, uint128 end);
void deleteObjArray(objData ** obj, int n_threads);





