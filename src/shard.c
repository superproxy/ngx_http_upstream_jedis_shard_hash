#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_map.h"
#include "m64aa.h"
#include "shard.h"

 
int shard_compare_int(void *x, void *y)
{ 
	return  (*((int64_t*)x) < *((int64_t*)y)) ? -1 : (*((int64_t*)x) == (*((int64_t*)y)) ? 0 : 1);
}

int shard_compare(void *x, void *y)
{ 
	tree_map_entry* entryX = (tree_map_entry*)x;
	tree_map_entry* entryY = (tree_map_entry*)y; 
	return (*((int64_t*)entryX->key) < *((int64_t*)entryY->key)) ? -1 : (*((int64_t*)entryX->key) == ( *((int64_t*)entryY->key)) ? 0 : 1);
}

char*  shard_itoa(int value, char* x) {
	if(x==NULL)
		return NULL;
	sprintf(x,"%d",value);
	return x;
}


shard* shard_select(shard_t* pshard_t,char* redis_key) {
	shard** shards  = pshard_t->shards;
	tree_map* map= pshard_t->tree_map;
	int64_t lredis_key = murmurhash64ac(redis_key);
	int64_t* pkey = &lredis_key;
	tree_map_entry* entry  = tree_map_tail(map, pkey, (Comp*)shard_compare_int);
	if (entry == NULL) {
		printf(" no find %ld\n", pkey);
		entry = tree_map_first(map);
	}
	//printf("select entry lkey %ld,hash:%s\n", *(int64_t*)entry->key, (char*)entry->value);
	int shard_size = pshard_t->shards_size; 
	int i; 
	for(i=0; i<shard_size;i++) {
        char* name = (char *)entry->value; 
		if(strcmp(name,shards[i]->name)==0) { 
			shard* x= shards[i];
			// printf("selected shard %s,weight:%d\n",x->name,x->weight);
			return x;
		}
	}
	return NULL;
}

void  
shard_init(shard_t* pshard_t, shard* shards [], int shard_size) {
	tree_map*  _tree_map = (tree_map*)malloc(sizeof(tree_map));
	pshard_t->tree_map = _tree_map;
	tree_map_init(pshard_t->tree_map, (Comp*)shard_compare); 
	
	pshard_t->shards = shards;
	pshard_t->shards_size = shard_size;  
  
	int i;
	for(i=0; i<shard_size;i++) {
		int weight = shards[i]->weight; 
		int n;
		for(n=0; n< 160*weight;n++) {
			char* name = shards[i]->name; 
			char  key[50]; 
			char a[10];
			char b[10];
			key[0] = '\0';
			a[0] = '\0';
			b[0] = '\0';
			if(name!=NULL){ 
				strcat(key,"SHARD-");
				strcat(key, shard_itoa(i, (char*)&a));
				strcat(key,"-NODE-");
				strcat(key, shard_itoa(n, (char*)&b));
			}
			else {
				strcat(key,name);
				strcat(key,"*"); 
				strcat(key, shard_itoa(weight, (char*) &a));
				strcat(key, shard_itoa(n, (char*) &b));
			} 
			int64_t lkey = murmurhash64ac(key); 
			
			tree_map_entry* entry = tree_map_entry_new(sizeof(int64_t), sizeof(name));
			memcpy(entry->key, &lkey, sizeof(int64_t));
			strcpy((char*)(entry->value), name);
			
			printf("lkey %ld,hash:%s\n", *(int64_t*)entry->key, (char*)entry->value);
			tree_map_put(pshard_t->tree_map, entry);
			
		}
	} 
} 

void  shard_free(shard_t*  pshard_t) { 
	tree_map_free(pshard_t->tree_map);
	free(pshard_t); 
}
