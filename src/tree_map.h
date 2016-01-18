#ifndef  _TREE_MAP_H_
#define  _TREE_MAP_H_  

#include "tree.h"

typedef struct
{
	void* key;
	void* value;
	
}tree_map_entry; 

typedef struct
{
	Red_black_tree* tree;
} tree_map;

extern void 
tree_map_init(tree_map* tree_map, Comp* cmp); 
extern void 
tree_map_put(tree_map* tree_map, void* object); 

extern tree_map_entry* 
tree_map_entry_new(int sizeKey, int sizeValue);
	
extern tree_map_entry* 
tree_map_first(tree_map* tree_map1);

extern void 
tree_map_free(tree_map* tree_map1);

extern  tree_map_entry*
 tree_map_tail(tree_map* tree_map1, void * key, Comp* cmp_key);

#endif

