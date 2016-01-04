#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include "m64aa.h"

int64_t
murmurhash64aa ( const void * key, int len, int64_t seed )
{
       int64_t m = 0xc6a4a7935bd1e995;
      //int64_t m = -4132994306676758123;
      printf("\n");
      printf("seed:%lld", seed);
      printf("\n");
      printf("m:%lld", m);
      printf("\n");
	const int r = 47;
	int64_t h = seed ^ (len * m);
      printf("h:%ld", h);
      printf("\n");
	const int64_t * data = (const int64_t *)key;
	const int64_t * end = data + (len/8);

	while(data != end)
	{
		int64_t k = *data++;

		k *= m; 
	        k ^= (0xffffffffffffffff&k) >> r;
		k *= m; 
		
		h ^= k;
		h *= m; 
      printf("while K:%ld", k);
      printf("\n");
      printf("while h:%ld", h);
      printf("\n");
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
          case 7: h ^= (int64_t)(data2[6]) << 48;
  case 6: h ^= (int64_t)(data2[5]) << 40;
  case 5: h ^= (int64_t)(data2[4]) << 32;
  case 4: h ^= (int64_t)(data2[3]) << 24;
  case 3: h ^= (int64_t)(data2[2]) << 16;
  case 2: h ^= (int64_t)(data2[1]) << 8;
  case 1: h ^= (int64_t)(data2[0]);
          h *= m;
	};


      printf("last h:%ld", h);
      printf("\n");
      printf("last r:%ld", r);
      printf("\n");
       
	//h ^= (0x0|h)>> r;
	h ^= (0xffffffffffffffff&h) >> r;
      printf("last h3:%ld", h);
      printf("\n");
	h *= m;
      printf("last h2:%ld", h);
      printf("\n");
        
//	h ^= (0x0|h) >> r;
	h ^= (0xffffffffffffffff&h) >> r;
      printf("last h1:%ld", h);
      printf("\n");
	return h;
} 
int64_t
murmurhash64ac ( const void * key ) {
    int len = 0;
    char *str = (char*)key;
    while (str[++len]) {}
    return  murmurhash64aa(str, len, 0x1234ABCD);
}

