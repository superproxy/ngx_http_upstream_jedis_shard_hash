#ifndef _SHARD_H_
#define _SHARD_H_

#include "rbtree_container.h"
#include "tree_map.h"
typedef struct {
	char *name;
	int weight;
} shard;

typedef struct {
	tree_map* tree_map;
	shard** shards;
	int shards_size;
} shard_t;

extern void
shard_init(shard_t* shard_t,shard* shards [], int shard_size);

extern void
shard_free(shard_t* shard_t);

extern shard* 
shard_select(shard_t* shard_t,char *redis_key); 
#endif

