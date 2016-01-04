local ffi = require "ffi"

ffi.cdef[[
typedef struct { char *name; int weight; } shard;
void shard_init(shard* shards[]);
shard* shard_select(shard*ss[], char *redis_key);
]]

