#include "base_test.h"
 

char*  itoa3(int value, char* x) 
{
  if(x==NULL)
     return NULL;
  sprintf(x,"%d",value); 
  return x;
} 

//  long jump  setjum buffer

 #ifdef   SHARD_TEST
	   int main(int argc, char* args[]) 
#else
	   int shard_test(int argc, char* args[])
#endif
{
    shard shard1;
    shard1.weight = 1;
    shard1.name="master1";

    shard shard2;
    shard2.name="master2";
    shard2.weight = 1;
	
    shard shards[] = {shard1,shard2};
    int shard_size = sizeof(shards)/sizeof(shard);
    int i;

    shard* ss[] = {&shard1,&shard2};
    for(i=0; i<shard_size; i++)
    {
        printf("selected:%s,weight:%d\n", ss[i]->name,ss[i]->weight);
    }

	shard_t   shard_t;
	shard_init(&shard_t, ss, shard_size);  
    char* redis_key ="test"; 
	rbtree_container tree;  
    char a[2000]; // 不是每次都重新定义，至少简便。不要放在for循环里面
    for(  i=0; i< 120; i++)
    {
        a[0]='\0';
    
        char c[10];
        itoa3(i,c);
        //printf("iiiiiiiiiiiiii:%d\n",i);
        //printf("aaaaaaaaaaaaaaaaaaaaa:%s\n",c);
        //printf("c:[10]len:%d\n",strlen(c));
       // printf("c:[10]value:%s\n",c);
        strcat(a,redis_key);
        strcat(a,c);
	    printf("a[200]len:%d,%s\n", strlen(a),a);
       // printf("a[200]len:%d\n",strlen(a));
       //  printf("a:%s\n",a);
       shard* shard=shard_select(&shard_t, a);
	    if (shard != NULL)
	    {
		    printf("selected:%s,weight:%d\n", shard->name, shard->weight);
	    }
	    else
	    {
		    printf("erro no found"); 
	    }
    }
	
	return 0;
}

