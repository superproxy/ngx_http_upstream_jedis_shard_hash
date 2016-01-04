#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h>

#include "rbtree_container.h"

#define TEST_SIZE 100000

#define SRAND 0x1234
//#define SRAND (time(NULL))

int compare(void *key1, void *key2)
{
	return *((int*)key1) - *((int*)key2); 
}

int main(void)
{
	rbtree_container tree;
	rbtree_container_node *pc, *apc[TEST_SIZE];
	int i, ret, insert_failed, find, find_failed, tree_size;
	struct timeval begin, end;
	
	rbtree_container_init(&tree, sizeof(int), compare);
	
	printf("rbtree_container benchmark go ... \n\n");
	
	srand(SRAND);
	
	/*
	 * 内存分配
	 */
	
	printf("malloc %d times...\n", TEST_SIZE);
	
	gettimeofday(&begin, NULL);
	
	for(i=0; i<TEST_SIZE; i++)
	{
		apc[i] = rbtree_container_node_malloc(&tree, 4);
	}
	
	gettimeofday(&end, NULL);
	printf("  time: %ldus\n", 1000000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec));
	
	printf("\n");
	/********************************************************/
	
	/*
	 * 批量内存分配
	 */
	
	printf("malloc array %d size...\n", TEST_SIZE);
	
	gettimeofday(&begin, NULL);
	
	pc = rbtree_container_node_array_malloc(&tree, 4, TEST_SIZE);
	
	gettimeofday(&end, NULL);
	printf("  time: %ldus\n", 1000000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec));
	
	rbtree_container_node_free(pc); //仅测试，释放了
	
	printf("\n");
	/********************************************************/
	
	//赋随机值
	for(i=0; i<TEST_SIZE; i++)
	{
		rbtree_container_node_set_key(apc[i], int, rand());
	}
	
	/********************************************************/
	
	/*
	 * 插入测试
	 */
	
	printf("random insert %d times...\n", TEST_SIZE);
	
	insert_failed = 0;
	
	gettimeofday(&begin, NULL);
	
	for(i=0; i<TEST_SIZE; i++)
	{
		ret = rbtree_container_insert(&tree, apc[i]);
		if (ret == -1)
			insert_failed ++;
	}
	
	gettimeofday(&end, NULL);
	
	printf("  insert_failed: %d\n", insert_failed);
	printf("  time: %ldus\n", 1000000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec));
	
	printf("\n");
	/********************************************************/
	
	/*
	 * 查找测试
	 */
	
	printf("random find %d times...\n", TEST_SIZE);
	 
	find_failed = 0;
	
	gettimeofday(&begin, NULL);
	
	for(i=0; i<TEST_SIZE; i++)
	{
		find = rand();
		pc = rbtree_container_search(&tree, &find);
		if (pc == NULL)
			find_failed ++;
	}
	
	gettimeofday(&end, NULL);
	
	printf("  find_failed: %d\n", find_failed);
	printf("  time: %ldus\n", 1000000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec));
		
	printf("\n");
	/********************************************************/
	
	/*
	 * 迭代器测试
	 */
	 
	printf("iterator...\n");
	 
	tree_size = 0;
	
	gettimeofday(&begin, NULL);
	
	pc = rbtree_container_first(&tree);
	while (pc)
	{
		tree_size ++;
		pc = rbtree_container_next(pc);
	}
	
	gettimeofday(&end, NULL);
	
	printf("  tree_size: %d\n", tree_size);
	printf("  time: %ldus\n", 1000000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec));
	
	printf("\n");
	/********************************************************/
	
	printf("rbtree_container benchmark end.\n");
		
	return 0;
} 
