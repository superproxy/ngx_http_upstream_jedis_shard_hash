### Nginx编译
./configure  --add-module=../ngx_http_upstream_jedisk_shard_hash
### Lua集成
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


### nginx使用
upstream {
host1;
host2;
jedis_shad_hash;
}


