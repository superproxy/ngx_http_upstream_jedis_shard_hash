#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

/*!
 * Container class for a red-black tree: A binary tree that satisfies the
 * following properties:
 * 1. Each node has a color, which is either red or black.
 * 2. A red node cannot have a red parent.
 * 3. The number of black nodes from every path from the tree root to a leaf
 *    is the same for all tree leaves (it is called the 'black depth' of the 
 *    tree).
 * Due to propeties 2-3, the depth of a red-black tree containing n nodes
 * is bounded by 2*log_2(n).
 *
 * The Red_black_tree template requires two template parmeters:
 * - The contained TYPE class represents the objects stored in the tree.
 *   It has to support the copy constructor and the assignment operator 
 *   (operator=).
 * - Comp is a functor used to define the order of objects of class TYPE:
 *   This class has to support an operator() that recieves two objects from
 *   the TYPE class and returns a negative, zero or a positive integer,
 *   depending on the comparison result.
 */

/*! Color enumeration for nodes of red-black tree */
typedef enum red_black_color {Red, Black} Red_black_color;

/*! Representation of a node in a red-black tree */
typedef struct red_black_node {
	void * object;                        /* the stored object */
	Red_black_color color;                /* the color of the node */
	struct red_black_node * parent;       /* points to the parent node */
	struct red_black_node * right;        /* points to the right child */
	struct red_black_node * left;         /* points to the left child */
} Red_black_node;

typedef int(Comp)(void *, void *);
typedef void(Op)(void *);
  
/*! Construct of a red-black tree node
 * \param object The object stored in the node
 * \param color The color of the node
 */
extern Red_black_node * rbnode_construct(void * object, Red_black_color color);

/*! Recursive destructor for the entire sub-tree */
extern void rbnode_destruct(Red_black_node * node);

/*! Calculate the depth of the sub-tree spanned by the given node
 * \param node The sub-tree root
 * \return The sub-tree depth
 */
extern int rbnode_depth(Red_black_node * node);

/*! Get the leftmost node in the sub-tree spanned by the given node
 * \param node The sub-tree root
 * \return The sub-tree minimum
 */
extern Red_black_node * rbnode_minimum(Red_black_node * node);

/*! Get the rightmost node in the sub-tree spanned by the given node
 * \param node The sub-tree root
 * \return The sub-tree maximum
 */
extern Red_black_node * rbnode_maximum(Red_black_node * node);

/*! Replace the object */
extern void rbnode_replace(Red_black_node * node, void * object);

/*! Get the next node in the tree (according to the tree order)
 * \param node The current node
 * \return The successor node, or NULL if node is the tree maximum
 */
extern Red_black_node * rbnode_successor(Red_black_node * node);

/*! Get the previous node in the tree (according to the tree order)
 * \param node The current node
 * \return The predecessor node, or NULL if node is the tree minimum
 */
extern Red_black_node * rbnode_predecessor(Red_black_node * node);

/*! Duplicate the entire sub-tree rooted at the given node
 * \param node The sub-tree root
 * \return A pointer to the duplicated sub-tree root
 */
extern Red_black_node * rbnode_duplicate(Red_black_node * node);

/*! Traverse a red-black sub-tree
 * \param node The sub-tree root
 * \param op The operation to perform on each object in the sub-tree
 */
extern void rbnode_traverse(Red_black_node * node, Op * op);

/*! Representation of a red-black tree */
typedef struct red_black_tree {
	Red_black_node * root;                /* pointer to the tree root */
	int iSize;                            /* number of objects stored */
	Comp * comp;                          /* compare function */
} Red_black_tree;

/*! Initialize a red-black tree with a comparision function
 * \param tree The tree
 * \param comp The comparision function
 */
void rbtree_init(Red_black_tree * tree, Comp * comp);

/*! Construct a red-black tree with a comparison object
 * \param comp A pointer to the comparison object to be used by the tree
 * \return The newly constructed  tree
 */
Red_black_tree * rbtree_construct(Comp * comp);

/*! Clean a red-black tree [takes O(n) operations]
 * \param tree The tree
 */
extern void rbtree_clean(Red_black_tree * tree);

/*! Destruct a red-black tree
 * \param tree The tree
 */
extern void rbtree_destruct(Red_black_tree * tree);

/*! Get the size of the tree [takes O(1) operations]
 * \param tree The tree
 * \return The number of objects stored in the tree
 */
extern int rbtree_size(Red_black_tree * tree);

/*! Get the depth of the tree [takes O(n) operations]
 * \param tree The tree
 * \return The length of the longest path from the root to a leaf node
 */
extern int rbtree_depth(Red_black_tree * tree);

/*! Check whether the tree contains an object [takes O(log n) operations]
 * \param tree The tree
 * \param object The query object
 * \return (true) if an equal object is found in the tree, otherwise (false)
 */
extern int rbtree_contains(Red_black_tree * tree, void * object);

/*! Insert an object to the tree [takes O(log n) operations]
 * \param tree The tree
 * \param object The object to be inserted
 * \return the inserted object node
 */
extern Red_black_node * rbtree_insert(Red_black_tree * tree, void * object);

/*! Insert a new object to the tree as the a successor of a given node
 * \param tree The tree
 * \return The new node
 */
extern Red_black_node * insert_successor_at(Red_black_tree * tree,
	Red_black_node * at_node,
	void * object);

	/*! Insert a new object to the tree as the a predecessor of a given node
	 * \param tree The tree
	 * \return The new node
	 */
extern Red_black_node * insert_predecessor_at(Red_black_tree * tree,
	Red_black_node * at_node,
	void * object);

	/*! Remove an object from the tree [takes O(log n) operations]
	 * \param tree The tree
	 * \param object The object to be removed
	 * \pre The object should be contained in the tree
	 */
extern void rbtree_remove(Red_black_tree * tree, void * object);

/*! Get a handle to the tree minimum [takes O(log n) operations]
 * \param tree The tree
 * \return the minimal object in the tree, or a NULL if the tree is empty
 */
extern Red_black_node * rbtree_minimum(Red_black_tree * tree);

/*! Get a handle to the tree maximum [takes O(log n) operations]
 * \param tree The tree
 * \return the maximal object in the tree, or a NULL if the tree is empty
 */
extern Red_black_node * rbtree_maximum(Red_black_tree * tree);

/*! Get the next node in the tree (according to the tree order)
 * [takes O(log n) operations at worst-case, but only O(1) amortized]
 * \param tree The tree
 * \param node The current object
 * \return The successor node, or a NULL, if we are at the tree maximum
 */
extern Red_black_node * rbtree_successor(Red_black_tree * tree,
	Red_black_node * node);

	/*! Get the previous node in the tree (according to the tree order)
	 * [takes O(log n) operations at worst-case, but only O(1) amortized]
	 * \param tree The tree
	 * \param node The current object
	 * \return The predecessor node, or a NULL, if we are at the tree minimum
	 */
extern Red_black_node * rbtree_predecessor(Red_black_tree * tree,
	Red_black_node * node);

	/*! Find a node that contains the given object
	 * \param tree The tree
	 * \param object The desired object
	 * \return A node that contains the given object, or NULL if no such object
	 * is found in the tree
	 */
extern Red_black_node * rbtree_find(Red_black_tree * tree, void * object);

/*! Remove the object stored in the given tree node 
 * \param tree The tree
 * \param node The node storing the object to be removed from the tree
 */
extern void rbtree_remove_at(Red_black_tree * tree, Red_black_node * node);

/*! Left-rotate the sub-tree spanned by the given node
 * \param tree The tree
 * \param node The sub-tree root
 */
extern void rbtree_rotate_left(Red_black_tree * tree, Red_black_node * node);

/*! Right-rotate the sub-tree spanned by the given node
 * \param tree The tree
 * \param node The sub-tree root
 */
extern void rbtree_rotate_right(Red_black_tree * tree, Red_black_node * node);

/*!
 * Fix-up the red-black tree properties after an insertion operation
 * \param tree The tree
 * \param node The node that has just been inserted to the tree
 * \pre The color of node must be red
 */
extern void rbtree_insert_fixup(Red_black_tree * tree, Red_black_node * node);

/*! Fix-up the red-black tree properties after a removal operation
 * \param tree The tree
 * \param node The child of the node that has just been removed from the tree
 */
extern void rbtree_remove_fixup(Red_black_tree * tree, Red_black_node * node);

/*! Traverse a red-black tree
 * \param tree The tree
 * \param op The operation to perform on every object of the tree (according to
 * the tree order)
 */
extern void rbtree_traverse(Red_black_tree * tree, Op * op);

#endif