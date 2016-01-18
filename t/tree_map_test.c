#include "base_test.h"


int entry_compare(void *x, void *y)
{ 
	tree_map_entry* entryX = (tree_map_entry*)x;
	tree_map_entry* entryY = (tree_map_entry*)y; 
	return (*((int*)entryX->key) < (*((int*)entryY->key))) ? -1 : (*((int*)entryX->key) == (*((int*)entryY->key)) ? 0 : 1);
}

int cmp_int2(void *a, void *b)
{
	return ((*(int*)a) < (*(int*)b)) ? -1 : (*(int*)a == *(int*)b) ? 0 : 1;
}

#ifdef   TREE_MAP_TEST
int main(int argc, char * argv [])   
#else
	int TREE_MAP_TEST(int argc, char * argv []) 
#endif 
	
{ 
	tree_map map;  
	tree_map_init(&map, (Comp*)entry_compare);  
	tree_map_entry *entry;  
 
	int i;
	char* a = "value";
	for (i = 0; i < 10; i++) {
		tree_map_entry* entry = tree_map_entry_new(sizeof(int), sizeof(a));
		memcpy(entry->key, &i, sizeof(i));
		 
		strcpy((char*)(entry->value), a);
		
		printf("entry->key:%d", *(int*)entry->key);
		printf("\n");  
		printf("entry->value:%s", (char*)entry->value);
		printf("\n");  
		tree_map_put(&map, entry);
	}
	
	int c;
	tree_map_entry* node;
	c = 5;
	printf("process tail c:%d", c);
	printf("\n");
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	printf("node->key:%d", *(int*)node->key);  //5
	printf("\n");
	
	c = 1;
	printf("process tail c:%d", c);
	printf("\n");
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	printf("node->key:%d", *(int*)node->key);  //1
	printf("\n");
	
	c = 9;
	printf("process tail c:%d", c);
	printf("\n");
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	if (node == NULL)
	{
		printf("NODENULL");
		printf("\n");
		node = tree_map_first(&map);
	}
	printf("node->key:%d", *(int*)node->key);  //5
	printf("\n");
	
	c = 10;
	printf("process tail c:%d", c);
	printf("\n"); 
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	if (node == NULL)
	{
		printf("NODENULL");
		printf("\n");
		node = tree_map_first(&map);
	}
	printf("node->key:%d", *(int*)node->key);  //5
	printf("\n");
	c = 100;
	printf("process tail c:%d", c);
	printf("\n"); 
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	if (node == NULL)
	{
		printf("NODENULL");
		printf("\n");
		node = tree_map_first(&map);
	}
	printf("node->key:%d", *(int*)node->key);  //5
	printf("\n");
	
	c = 120;
	printf("process tail c:%d", c);
	printf("\n"); 
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	if (node == NULL)
	{
		printf("NODENULL");
		printf("\n");
		node = tree_map_first(&map);
	}
	printf("node->key:%d", *(int*)node->key);  //5
	printf("\n");
	
	c = -1;
	printf("process tail c:%d", c);
	printf("\n");
	node = tree_map_tail(&map, (void*)&c, (Comp*) cmp_int2);
	if (node == NULL)
	{
		printf("NODENULL");
		printf("\n");
		node = tree_map_first(&map);
	}
	printf("node->key:%d", *(int*)node->key);  //0
	printf("\n"); 
	
	return 0;
}
	
	