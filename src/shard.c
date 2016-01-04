#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rbtree_container.h"
#include "m64aa.h"
#include "shard.h"


rbtree_container tree;
//int64_t shard_compare(void *x, void *y)
void* shard_compare(void *x, void *y)
{
	// return (x < y) ? -1 : ((x == y) ? 0 : 1)
	return (void *) ( *((int64_t*)x) - *((int64_t*)y)); 
}

char*  itoa2(int value, char* x) {
	if(x==NULL)
		return NULL;
	sprintf(x,"%d",value);
	return x;
}


shard* shard_select(shard* shards[],char* redis_key) {
	int64_t lredis_key = murmurhash64ac(redis_key);
	int64_t * pkey = &lredis_key;
	rbtree_container_node *pc  = rbtree_container_abslowest(&tree, pkey);
	if (pc == NULL) {
	//	printf("  no find %ld\n", pkey);
		pc =rbtree_container_first(&tree);
	}
//	printf("  key %ld pc data: %s\n", *pkey, (char*)(pc->data));
	int shard_size = sizeof(*shards)/sizeof(shards[0]);
	int i;
	for(i=0; i<shard_size;i++) {
		char* name = (char *)pc->data;
		if(strcmp(name,shards[i]->name)==0) { 
			shard* x= shards[i];
//			printf("selected shard %s,weight:%d",x->name,x->weight);
			//return shards[i];
			return x;
		}
	}
	return NULL;
}

void  shard_init(shard* shards[]) {
	rbtree_container_init(&tree, sizeof(int64_t), shard_compare);
	rbtree_container_node *pc ;

	//int shard_size = strlen(shards);
	//	printf("sizeof shards%d\n", sizeof(*shards));
//	printf("sizeof shard[0]%d\n", sizeof(shards[0]));
	int shard_size = sizeof(*shards)/sizeof(shards[0]);
	//printf("shard_size%d\n", shard_size);
	int i;
	for(i=0; i<shard_size;i++) {
		int weight = shards[i]->weight;
//		printf("weight%d\n", weight);
		int n;
		for(n=0; n< 160*weight;n++) {
			char* name = shards[i]->name;
			printf("name:%s\n", name);
			char* key=(char*)malloc(sizeof(char)*50);
			if(name!=NULL){
				printf("in\n");
//				//sprintf(key,"%s%d%s%d","SHARD-",i,"-NODE-",n);
				char a[10];
				char b[10];
				strcat(key,"SHARD-");
				strcat(key,itoa2(i,&a));
				strcat(key,"-NODE-");
				strcat(key,itoa2(n,&b));
			}
			else {
//				printf("else\n");
				//sprintf(key,"%s%s%d%d", name, "*", weight,n);
				strcat(key,name);
				strcat(key,"*");
				char a[10];
				char b[10];
				strcat(key,itoa2(weight,&a));
				strcat(key,itoa2(n,&b));
			}

			//key="hello";
			int64_t lkey = murmurhash64ac(key);
//			printf("hash:%s,lkey %ld",key,lkey);
			pc = rbtree_container_node_malloc(&tree, sizeof(name));
			rbtree_container_node_set_key(pc, int64_t, lkey); 
			strcpy((char*)(pc->data), name);
			rbtree_container_insert(&tree, pc);
		}
	}
}
