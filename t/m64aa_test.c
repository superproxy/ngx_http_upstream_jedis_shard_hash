#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include "m64aa.h"

int main(int argc, char **argv)
{
    char *str = argv[1];
    printf("str:%s",str);
    int len = 0;
    printf("\n");
    while (str[++len]) {}
    long  hash = murmurhash64aa(str, len, 0x1234ABCD);
    printf("%d",sizeof(hash));
    printf("\n");
    
    uint64_t a; 
    printf("%d",sizeof(a));
    printf("\n");
    long long  b; 
    printf("%d",sizeof(b));
    printf("\n");
    printf("result:%lld", hash);
    printf("result:%ld", hash);
    printf("result:%l", hash);
    printf("result:%lu", hash);
    printf("\n");
    return 0;
}
