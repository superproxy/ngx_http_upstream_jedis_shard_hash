###  COMPILE WITH NGINX
./configure  --add-module=../ngx_http_upstream_jedisk_shard_hash
### LUA USAGE

##1. put shard.lua into the folder of your lua libs

##2. config the file
```
location =/s {
                        default_type 'text/plain';
                        content_by_lua ' 
                                local shard= require "shard"
                                local ffi= require "ffi"
                                local shards=ffi.new("shard [?]",1);
                                local pshards=ffi.new("shard* [?]",1);
                                shards[0].name=ffi.cast("char *","master");
                                shards[0].weight=1;
                                pshards[0]=ffi.cast("shard*",shards[0]);
                                ffi.C.shard_init(pshards);
                                local selected= ffi.C.shard_select(pshards,ffi.cast("char *","test"));                                ngx.say(ffi.string(selected.name));
                                ngx.say(selected.weight);
                        ';          
                }           
}

```
### NGINX USAGE
```
upstream {
host1;
host2;
jedis_shad_hash;
}
``


