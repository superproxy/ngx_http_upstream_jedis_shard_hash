local _SHARD = {
}

local ffi = require "ffi"
ffi.cdef[[
typedef struct { char *name; int weight; } shard;
typedef struct { void* tree_map; shard** shards; int shard_size;} shard_t;
void shard_init(shard_t* shart_t,shard* shards[],int shard_size);
shard* shard_select(shard_t* shard_t, char *redis_key);
]]

_SHARD.initialized = false
_SHARD.pshard_t={}

function _SHARD.select(key)
	local selected= ffi.C.shard_select(_SHARD.pshard_t,ffi.cast("char *",key))
	local server=ffi.string(ffi.cast("char *",selected.name))
	return server
end

function _SHARD.init(shardInfos)
	if  _SHARD.initialized then
		return 
	end
	local shard_t=ffi.new("shard_t[?]",1)
	_SHARD.pshard_t = shard_t[0]
	local size =  2;
	local shards=ffi.new("shard [?]",size)
	local pshards=ffi.new("shard* [?]",size)
	for i,shard in ipairs(shardInfos) do
		shards[i-1].name=ffi.cast("char *",shard.name)
		shards[i-1].weight=shard.weight
		pshards[i-1]=ffi.cast("shard*",shards[i-1])
	end
	ffi.C.shard_init(_SHARD.pshard_t,pshards,size)
	_SHARD.initialized=true;
end

return _SHARD
