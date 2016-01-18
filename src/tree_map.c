#include "tree_map.h" 
#include "tree.h"
#include "stdio.h"


 void tree_map_init(tree_map* tree_map, Comp* cmp)
{
	tree_map->tree =  (Red_black_tree*)malloc(sizeof(Red_black_tree)); //释放节点内部的
	rbtree_init(tree_map->tree, (Comp*) cmp);
}

// object  entry
void tree_map_put(tree_map* tree_map, void* object)
{ 
	rbtree_insert(tree_map->tree, object); 
}

void tree_map_free(tree_map* tree_map)
{ 
	rbtree_clean(tree_map->tree); 
	free(tree_map->tree);   //  
}
tree_map_entry*  tree_map_entry_new(int sizeKey, int sizeValue)
{
	// 分配空间,释放通过列表逐个释放
	// 一次性分配好
	tree_map_entry*	entry = (tree_map_entry*)malloc(sizeof(tree_map_entry) + sizeKey + sizeValue);
	entry->key = (void*)( ((char*)entry) + sizeof(tree_map_entry));
	entry->value = (void*)(((char*)entry) + sizeof(tree_map_entry) + sizeKey);
	return entry;  
} 


tree_map_entry*  tree_map_first(tree_map* tree_map) 
{ 
	Red_black_node*	node  = tree_map->tree->root; 
	if (node != NULL)
	{  
		while (node->left != NULL)
		{
			node = node->left;  
		}
	}
			
	return  (tree_map_entry*) (node->object);
}

	//  map中查找最小一个，降序 
tree_map_entry *tree_map_tail(tree_map* tree_map, void * key, Comp* comp_key)
{ 
	Red_black_node*	node  = tree_map->tree->root;
	tree_map_entry*  entry;
	while (node != NULL) 
	{
		entry =  (tree_map_entry*) (node->object);
		int result = comp_key(key, entry->key);
		if (result == 0) { 
			//printf("%s", "lowest,0");
			return entry;
		}
		else if (result < 0) {
			if (node->left != NULL) {
				node = node->left;
			}
			else {
						//printf("%s", "lowest,a");
				return entry;
			}
		}
		else if (result > 0) {
			if (node->right != NULL) {
				node = node->right;
			}
			else {
				Red_black_node*  parent = node->parent;
				Red_black_node*  x = node;
				while (parent != NULL && x == parent->right) {
					x = parent;
					parent = parent->parent;
				}
			 
				//printf("%s", "lowest,b");
				if (parent == NULL)
				{
					return NULL;
				}
				return (tree_map_entry*)parent->object;
			}
		}
	}
	return NULL;
}



