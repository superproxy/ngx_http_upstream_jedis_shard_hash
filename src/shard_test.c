#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rbtree_container.h"
#include "m64aa.h"
#include "shard.h"

shard*  shard_select2(shard*ss[], char *redis_key) {
	return ss[0];
}

int main(void)
{
	shard shard1;
	shard1.weight = 1;
	shard1.name="master1";
	shard shards[] = {shard1};
	int shard_size = sizeof(shards)/sizeof(shards[0]);
	printf("shards%d\n", sizeof(shards));
	printf("shards[0]%d\n", sizeof(shards[0]));
	printf("%d", shard_size);
	printf("\n");
	int i;
	for(i=0;i<shard_size;i++) {
		printf("%s", shards[i].name);
		printf("\n");
		printf("%d", shards[i].weight);
		printf("\n");
	}
	shard* ss[] = {&shard1};
	for(i=0;i<shard_size;i++) {
		printf("aaaaa%s", ss[i]->name);
		printf("\n");
		printf("aaa%d", ss[i]->weight);
		printf("\n");
	}

	shard_init(ss);
	char* redis_key ="test";
	shard*  s_shard = shard_select(ss,redis_key);
	if(s_shard!=NULL) {
		printf("OKn");
		printf("selected:%s", s_shard->name);
		printf("\n");
		printf("selected:%d", s_shard->weight);
		printf("\n");
	}
} 
