###  COMPILE WITH NGINX
./configure  --add-module=../ngx_http_upstream_jedis_shard_hash
### LUA USAGE
#####1. put shard.lua into the folder of your lua libs
#####2. config the file
```
lua_package_path "/usr/local/nginx/conf/?.lua;;";
upstream  master0 {
		server 127.0.0.1:6379;
		server 127.0.0.2:6379;
}
upstream  master1 {
		server 127.0.0.1:6380;
		server 127.0.0.2:6380;
}
server {
		location /redis_shard {
				internal;
				redis2_query  get  $arg_key;
				redis2_pass   $arg_upstream_server;
		}
		location /shard2 {
				default_type 'text/plain';
				content_by_lua ' 
						local s= require "shard"
						if  s.initialized==false  then
								shardInfos= {
										{name="master0", weight=1},
										{name="master1", weight=1},
								}
            				s.init(shardInfos)
		     				s.initialized=true
						end
						local akey= ngx.var.arg_key
						ngx.say(akey)
						local aserver=s.select(akey)
						ngx.say(aserver)
						local res= ngx.location.capture("/redis_shard", { args= {key=akey,upstream_server=aserver}})
						if  res.status==200 then
			                ngx.say(res.body)
						end
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
```


