#ifndef _M64AA_H_
#define _M64AA_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>

extern int64_t
murmurhash64aa ( const void * key, int len, int64_t seed );
extern int64_t
murmurhash64ac ( const void * key);
#endif
