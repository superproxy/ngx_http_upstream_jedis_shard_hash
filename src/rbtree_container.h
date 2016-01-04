/*
 *
 * Name:	红黑树通用容器（linux内核封装、纯C）
 * Auther:	chishaxie
 * Date:	2012.7.29
 *
 * Description:	log(n)的插入、删除、查询，带迭代器功能
 *
 */
#ifndef _RBTREE_CONTAINER_H_
#define _RBTREE_CONTAINER_H_

#include "rbtree.h"

typedef struct
{
	struct rb_node rb_node; //rbtree内核结构（必须为首个字段）
	size_t dlen; //data的长度
	void *key; //指向键（键定长）
	unsigned char *data; //指向值
} rbtree_container_node; //节点

typedef int (*key_compare)(void *key1, void *key2);

typedef struct
{
	struct rb_root root; //rbtree内核结构
	size_t klen; //key的长度（定长key）
	key_compare compare; //key的比较函数
} rbtree_container; //容器

#define rbtree_container_node_set_key(pnode,type,value) (*((type*)((pnode)->key)) = (value))

void rbtree_container_init(rbtree_container *tree, size_t key_length, key_compare key_compare_function);

rbtree_container_node* rbtree_container_search(rbtree_container *tree, void *key);
rbtree_container_node* rbtree_container_abslowest(rbtree_container *tree, void *key);
rbtree_container_node* rbtree_container_sub(rbtree_container *tree, void *key);

int rbtree_container_insert(rbtree_container *tree, rbtree_container_node *cont);

rbtree_container_node *rbtree_container_delete(rbtree_container *tree, void *key);

rbtree_container_node *rbtree_container_replace(rbtree_container *tree, rbtree_container_node *cont);

rbtree_container_node *rbtree_container_first(rbtree_container *tree);

rbtree_container_node *rbtree_container_last(rbtree_container *tree);

rbtree_container_node *rbtree_container_next(rbtree_container_node *cont);

rbtree_container_node *rbtree_container_prev(rbtree_container_node *cont);

void rbtree_container_erase(rbtree_container *tree, rbtree_container_node *cont);

rbtree_container_node *rbtree_container_node_malloc(rbtree_container *tree, size_t data_length);

rbtree_container_node *rbtree_container_node_array_malloc(rbtree_container *tree,
	size_t data_length, size_t array_length);

void rbtree_container_node_free(rbtree_container_node *tree_node);

#endif
