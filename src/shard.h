#ifndef _SHARD_H_
#define _SHARD_H_

typedef struct shard {
     char *name;
     int weight;
} shard;

extern void
shard_init(shard* shards[]);

extern shard* 
shard_select(shard*ss[], char *redis_key);

#endif

