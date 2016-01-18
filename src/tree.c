#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

/*
 * Operations on Red_black_node struct
 */

/* Construct a red-black tree node */
Red_black_node * rbnode_construct(void * object, Red_black_color color)
{
	Red_black_node * node = (Red_black_node *) malloc(sizeof(Red_black_node));
	if (!node) {
		fprintf(stderr, "Not enough memory!\n");
		return NULL;
	}
	node->object = object;
	node->color = color;
	node->parent = node->right = node->left = NULL;
	return node;
}

/* Destructor of a red-black tree node */
void rbnode_destruct(Red_black_node * node)
{
	if (!node) return;
	rbnode_destruct(node->right);
	rbnode_destruct(node->left);
	free(node);
}

/* Calculate the depth of the subtree spanned by a given node */
int rbnode_depth(Red_black_node * node)
{
  /* Recursively calculate the depth of the left and right sub-trees */
	int  iRightDepth = (node->right) ? rbnode_depth(node->right) : 0;
	int  iLeftDepth = (node->left) ? rbnode_depth(node->left) : 0;

	  /* Return the maximal child depth + 1 (the current node) */
	return ((iRightDepth > iLeftDepth) ? (iRightDepth + 1) : (iLeftDepth + 1));
}

/* Return the leftmost leaf in the tree */
Red_black_node * rbnode_minimum(Red_black_node * node)
{
	while (node->left)
		node = node->left;
	return node;
}

/* Return the rightmost leaf in the tree */
Red_black_node * rbnode_maximum(Red_black_node * node)
{
	while (node->right)
		node = node->right;
	return node;
}

/* Replace the object */
void rbnode_replace(Red_black_node * node, void * object)
{
  /* Make sure the replacement does not violate the tree order */
  
  /* Replace the object at node */
	node->object = object;
}
        
/* Get the next node in the tree (according to the tree order) */
Red_black_node * rbnode_successor(Red_black_node * node)
{
	Red_black_node * succ_node;

	if (node->right) {
	  /* If there is a right child, the successor is the minimal object in 
	   * the sub-tree spanned by this child.
	   */
		succ_node = node->right;
		while (succ_node->left)
			succ_node = succ_node->left;
	}
	else {
	  /* Otherwise, go up the tree until reaching the parent from the left 
	   * direction.
	   */
		Red_black_node * prev_node = node;
		succ_node = node->parent;
		while (succ_node && prev_node == succ_node->right) {
			prev_node = succ_node;
			succ_node = succ_node->parent;
		}
	}

	return (succ_node);
}

/* Get the previous node in the tree (according to the tree order) */
Red_black_node * rbnode_predecessor(Red_black_node * node)
{
	Red_black_node * pred_node;

	if (node->left) {
	  /* If there is a left child, the predecessor is the maximal object in 
	   * the sub-tree spanned by this child.
	   */
		pred_node = node->left;
		while (pred_node->right)
			pred_node = pred_node->right;
	}
	else {
	  /* Otherwise, go up the tree until reaching the parent from the right 
	   * direction.
	   */
		Red_black_node * prev_node = node;
		pred_node = node->parent;
		while (pred_node && prev_node == pred_node->left) {
			prev_node = pred_node;
			pred_node = pred_node->parent;
		}
	}

	return (pred_node);
}

/* Return a pointer to a duplication of the given node */
Red_black_node * rbnode_duplicate(Red_black_node * node)
{
  /* Create a node of the same color, containing the same object */
	Red_black_node * dup_node = rbnode_construct(node->object, node->color);
	if (!dup_node) return NULL;
  
	/* Duplicate the children recursively */
	if (node->right) {
		dup_node->right = rbnode_duplicate(node->right);
		dup_node->right->parent = dup_node;
	}
	else {
		dup_node->right = NULL;
	}

	if (node->left) {
		dup_node->left = rbnode_duplicate(node->left);
		dup_node->left->parent = dup_node;
	}
	else {
		dup_node->left = NULL;
	}

	return dup_node;                      /* Return the duplicated node */
}

/* Traverse a red-black subtree */
void rbnode_traverse(Red_black_node * node, Op * op)
{
	if (!node) return;
	rbnode_traverse(node->left, op);
	op(node->object);
	rbnode_traverse(node->right, op);
}

/*
 * Operations on Red_black_tree struct
 */

/* Intialize a tree */
void rbtree_init(Red_black_tree * tree, Comp * comp)
{
	tree->comp = comp;
	tree->iSize = 0;
	tree->root = NULL;
}

/* Construct a tree given a comparison function */
Red_black_tree * rbtree_construct(Comp * comp)
{
	Red_black_tree * tree = (Red_black_tree *) malloc(sizeof(Red_black_tree));
	if (!tree) {
		fprintf(stderr, "Not enough memory!\n");
		return NULL;
	}
	rbtree_init(tree, comp);
	return tree;
}

/* Remove all objects from a black-red tree */
void rbtree_clean(Red_black_tree * tree)
{
	if (tree->root)
		rbnode_destruct(tree->root);
	tree->root = NULL;
	tree->iSize = 0;
}

/* Destruct a red-black tree */
void rbtree_destruct(Red_black_tree * tree)
{
	rbtree_clean(tree);
	free(tree);
}

/* Returns the size of the tree */
int rbtree_size(Red_black_tree * tree)
{
	return tree->iSize;
}

/* Returns the depth of the tree */
int rbtree_depth(Red_black_tree * tree)
{
	if (!(tree->root))
		return 0;
	return rbnode_depth(tree->root);
}

/* Check whether the tree contains an object */
int rbtree_contains(Red_black_tree * tree, void * object)
{
	return (rbtree_find(tree, object) != NULL);
}

/* Insert an object to the tree */
Red_black_node * rbtree_insert(Red_black_tree * tree, void * object)
{
	Red_black_node * cur_node;
	Red_black_node * new_node;
  
	if (!(tree->root)) {
	  /* Assign a new root node. Notice that the root is always black */
		new_node = rbnode_construct(object, Black);
		if (!new_node) return NULL;
		tree->root = new_node;
		tree->iSize = 1;
		return new_node;
	}

	  /* Find a place for the new object, and insert it as a red leaf */
	cur_node = tree->root;
	new_node = rbnode_construct(object, Red);
	if (!new_node) return NULL;

	while (cur_node) {
	  /* Compare inserted object with the object stored in the current node */
		if ((*(tree->comp))(object, cur_node->object) < 0) {  // 大值放在right
			if (!(cur_node->left)) {
		  /* Insert the new leaf as the left child of the current node */
				cur_node->left = new_node;
				new_node->parent = cur_node;
				cur_node = NULL;                /* terminate the while loop */
			}
			else {
				cur_node = cur_node->left;      /* Go to the left sub-tree */
			}
		}
		else {
			if (!(cur_node->right)) {
		  /* Insert the new leaf as the right child of the current node */
				cur_node->right = new_node;
				new_node->parent = cur_node;
				cur_node = NULL;                /* terminate the while loop */
			}
			else {
				cur_node = cur_node->right;     /* Go to the right sub-tree */
			}
		}
	}

	  /* Mark that a new node was added */
	tree->iSize++;

	  /* Fix up the tree properties */
	rbtree_insert_fixup(tree, new_node);  

	return new_node;
}

/* Insert a new object to the tree as the a successor of a given node */
Red_black_node * insert_successor_at(Red_black_tree * tree,
	Red_black_node * at_node,
	void * object)
{
	Red_black_node * parent;
	Red_black_node * new_node;
  
	if (!(tree->root)) {
	  /* Assign a new root node. Notice that the root is always black */
		new_node = rbnode_construct(object, Black);
		if (!new_node) return NULL;
		tree->root = new_node;
		tree->iSize = 1;
		return new_node;
	}

	  /* Insert the new object as a red leaf, being the successor of node */
	new_node = rbnode_construct(object, Red);
	if (!new_node) return NULL;

	if (!at_node) {
	  /* The new node should become the tree minimum: Place is as the left
	   * child of the current minimal leaf.
	   */
		parent = rbnode_minimum(tree->root);
		parent->left = new_node;
	}
	else {
	  /* Make sure the insertion does not violate the tree order */

	      /* In case given node has no right child, place the new node as its 
	       * right child. Otherwise, place it at the leftmost position at the
	       * sub-tree rooted at its right side.
	       */
		if (!at_node->right) {
			parent = at_node;
			parent->right = new_node;
		}
		else {
			parent = rbnode_minimum(at_node->right);
			parent->left = new_node;
		}
	}

	new_node->parent = parent;

	  /* Mark that a new node was added */
	tree->iSize++;

	  /* Fix up the tree properties */
	rbtree_insert_fixup(tree, new_node);  

	return new_node;
}

/* Insert a new object to the tree as the a predecessor of a given node */
Red_black_node * insert_predecessor_at(Red_black_tree * tree,
	Red_black_node * at_node,
	void * object)
{
	Red_black_node * parent;
	Red_black_node * new_node;
  
	if (!(tree->root)) {
	  /* Assign a new root node. Notice that the root is always black */
		new_node = rbnode_construct(object, Black);
		if (!new_node) return NULL;
		tree->root = new_node;
		tree->iSize = 1;
		return new_node;
	}

	  /* Insert the new object as a red leaf, being the predecessor of at_node */
	new_node = rbnode_construct(object, Red);
	if (!new_node) return NULL;
  
	if (!at_node) {
	  /* The new node should become the tree maximum: Place is as the right
	   * child of the current maximal leaf.
	   */
		parent = rbnode_maximum(tree->root);
		parent->right = new_node;
	}
	else {
	  /* Make sure the insertion does not violate the tree order */

	      /* In case given node has no left child, place the new node as its 
	       * left child. Otherwise, place it at the rightmost position at the
	       * sub-tree rooted at its left side.
	       */
		if (!(at_node->left)) {
			parent = at_node;
			parent->left = new_node;
		}
		else {
			parent = rbnode_maximum(at_node->left);
			parent->right = new_node;
		}
	}

	new_node->parent = parent;

	  /* Mark that a new node was added */
	tree->iSize++;

	  /* Fix up the tree properties */
	rbtree_insert_fixup(tree, new_node);  

	return new_node;
}

/* Remove an object from the tree */
void rbtree_remove(Red_black_tree * tree, void * object)
{
	Red_black_node * node = rbtree_find(tree, object);    /* find the node */
	rbtree_remove_at(tree, node);                         /* remove the node */
}

/* Remove the object pointed by the given node. */
void rbtree_remove_at(Red_black_tree * tree, Red_black_node * node)
{
	Red_black_node * child = NULL;

	  /* In case of deleting the single object stored in the tree, free the root,
	   * thus emptying the tree.
	   */
	if (tree->iSize == 1) {
		rbnode_destruct(tree->root);
		tree->root = NULL;
		tree->iSize = 0;
		return;
	}

	  /* Remove the given node from the tree */
	if (node->left && node->right) {
	  /* If the node we want to remove has two children, find its successor,
	   * which is the leftmost child in its right sub-tree and has at most
	   * one child (it may have a right child).
	   */
		Red_black_node * succ_node = rbnode_minimum(node->right);

		    /* Now physically swap node and its successor. Notice this may temporarily
		     * violate the tree properties, but we are going to remove node anyway.
		     * This way we have moved node to a position were it is more convinient
		     * to delete it.
		     */
		int immediate_succ = (node->right == succ_node);
		Red_black_node * succ_parent = succ_node->parent;
		Red_black_node * succ_left = succ_node->left;
		Red_black_node * succ_right = succ_node->right;
		Red_black_color succ_color = succ_node->color;

		succ_node->parent = node->parent;
		succ_node->left = node->left;
		succ_node->right = immediate_succ ? node : node->right;
		succ_node->color = node->color;

		node->parent = immediate_succ ? succ_node : succ_parent;
		node->left = succ_left;
		node->right = succ_right;
		node->color = succ_color;

		if (!immediate_succ) { 
			if (succ_node == node->parent->left)
				node->parent->left = node;
			else
				node->parent->right = node;
		}

		if (node->left)
			node->left->parent = node;
		if (node->right)
			node->right->parent = node;

		if (succ_node->parent) {
			if (node == succ_node->parent->left)
				succ_node->parent->left = succ_node;
			else
				succ_node->parent->right = succ_node;
		}
		else {
			tree->root = succ_node;
		}

		if (succ_node->left)
			succ_node->left->parent = succ_node;
		if (succ_node->right)
			succ_node->right->parent = succ_node;
	}

	  /* At this stage, the node we are going to remove has at most one child */
	child = (node->left) ? node->left : node->right;

	  /* Splice out the node to be removed, by linking its parent straight to the 
	   * removed node's single child.
	   */
	if (child)
		child->parent = node->parent;
    
	if (!(node->parent)) {
	  /* If we are deleting the root, make the child the new tree node */
		tree->root = child;
	}
	else {
	  /* Link the removed node parent to its child */
		if (node == node->parent->left) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}
	}

	  /* Fix-up the red-black properties that may have been damaged: If we have
	   * just removed a black node, the black-depth property is no longer valid.
	   */
	if (node->color == Black && child)
		rbtree_remove_fixup(tree, child);

		  /* Delete the un-necessary node (we nullify both its children because the 
		   * node's destructor is recursive).
		   */
	node->left = NULL;
	node->right = NULL;
	free(node);

	  /* Descrease the number of objects in the tree */
	tree->iSize--;
}

/* Get the tree minimum */
Red_black_node * rbtree_minimum(Red_black_tree * tree)
{
	if (!(tree->root))
		return NULL;

		  /* Return the leftmost leaf in the tree */
	return rbnode_minimum(tree->root);
}

/* Get the tree maximum */
Red_black_node * rbtree_maximum(Red_black_tree * tree)
{
	if (!(tree->root))
		return NULL;

		  /* Return the rightmost leaf in the tree */
	return rbnode_maximum(tree->root);
}

/* Return a pointer to the node containing the given object */
Red_black_node * rbtree_find(Red_black_tree * tree, void * object)
{
	Red_black_node * cur_node = tree->root;
	int comp_result;

	while (cur_node) {
	  /* In case of equality, we can return the current node. */
		if ((comp_result = (*(tree->comp))(object, cur_node->object)) == 0)
			return cur_node;
		  /* Go down to the left or right child. */
		cur_node = (comp_result > 0) ? cur_node->left : cur_node->right;
	}

	  /* If we reached here, the object is not found in the tree */
	return NULL;
}

/* Left-rotate the sub-tree spanned by the given node:
 *
 *          |          RoateRight(y)            |
 *          y         -------------->           x
 *        /   \                               /   \       .
 *       x     T3       RoatateLeft(x)       T1    y      .
 *     /   \          <--------------            /   \    .
 *    T1    T2                                  T2    T3
 */
void rbtree_rotate_left(Red_black_tree * tree, Red_black_node * x_node)
{
  /* Get the right child of the node */
	Red_black_node * y_node = x_node->right;

	  /* Change its left subtree (T2) to x's right subtree */
	x_node->right = y_node->left;

	  /* Link T2 to its new parent x */
	if (y_node->left != NULL)
		y_node->left->parent = x_node;
    
	  /* Assign x's parent to be y's parent */
	y_node->parent = x_node->parent;

	if (!(x_node->parent)) {
	  /* Make y the new tree root */
		tree->root = y_node;
	}
	else {
	  /* Assign a pointer to y from x's parent */
		if (x_node == x_node->parent->left) {
			x_node->parent->left = y_node;
		}
		else {
			x_node->parent->right = y_node;
		}
	}

	  /* Assign x to be y's left child */
	y_node->left = x_node;
	x_node->parent = y_node;
}

/* Right-rotate the sub-tree spanned by the given node */
void rbtree_rotate_right(Red_black_tree * tree, Red_black_node * y_node)
{
  /* Get the left child of the node */
	Red_black_node * x_node = y_node->left;

	  /* Change its right subtree (T2) to y's left subtree */
	y_node->left = x_node->right;

	  /* Link T2 to its new parent y */
	if (x_node->right != NULL)
		x_node->right->parent = y_node;
    
	  /* Assign y's parent to be x's parent */
	x_node->parent = y_node->parent;

	if (!(y_node->parent)) {
	  /* Make x the new tree root */
		tree->root = x_node;
	}
	else {
	  /* Assign a pointer to x from y's parent */
		if (y_node == y_node->parent->left) {
			y_node->parent->left = x_node;
		}
		else {
			y_node->parent->right = x_node;
		}
	}

	  /* Assign y to be x's right child */
	x_node->right = y_node;
	y_node->parent = x_node;
}

/* Fix-up the tree so it maintains the red-black properties after insertion */
void rbtree_insert_fixup(Red_black_tree * tree, Red_black_node * node)
{
  /* Fix the red-black propreties: we may have inserted a red leaf as the 
   * child of a red parent - so we have to fix the coloring of the parent 
   * recursively.
   */
	Red_black_node * curr_node = node;
	Red_black_node * grandparent;
	Red_black_node *uncle;

	assert(node && node->color == Red);
  
	while (curr_node != tree->root && curr_node->parent->color == Red) {
	  /* Get a pointer to the current node's grandparent (notice the root is 
	   * always black, so the red parent must have a parent).
	   */
		grandparent = curr_node->parent->parent;
        
		if (curr_node->parent == grandparent->left) {
		  /* If the red parent is a left child, the uncle is the right child of 
		   * the grandparent.
		   */
			uncle = grandparent->right;

			if (uncle && uncle->color == Red) {
		  /* If both parent and uncle are red, color them black and color the 
			   * grandparent red.
			   * In case of a NULL uncle, we treat it as a black node.
			   */
				curr_node->parent->color = Black;
				uncle->color = Black;
				grandparent->color = Red;

					/* Move to the grandparent */
				curr_node = grandparent;
			}
			else {
		  /* Make sure the current node is a right child. If not, left-rotate 
			   * the parent's sub-tree so the parent becomes the right child of the 
			   * current node (see _rotate_left).
			   */
				if (curr_node == curr_node->parent->right) {
					curr_node = curr_node->parent;
					rbtree_rotate_left(tree, curr_node);
				}

					/* Color the parent black and the grandparent red */
				curr_node->parent->color = Black;
				grandparent->color = Red;

					/* Right-rotate the grandparent's sub-tree */
				rbtree_rotate_right(tree, grandparent);
			}
		}
		else {
		  /* If the red parent is a right child, the uncle is the left child of 
		   * the grandparent.
		   */
			uncle = grandparent->left;

			if (uncle && uncle->color == Red) {
		  /* If both parent and uncle are red, color them black and color the 
			   * grandparent red.
			   * In case of a NULL uncle, we treat it as a black node.
			   */
				curr_node->parent->color = Black;
				uncle->color = Black;
				grandparent->color = Red;

					/* Move to the grandparent */
				curr_node = grandparent;
			}
			else {
		  /* Make sure the current node is a left child. If not, right-rotate 
			   * the parent's sub-tree so the parent becomes the left child of the 
			   * current node.
			   */
				if (curr_node == curr_node->parent->left) {
					curr_node = curr_node->parent;
					rbtree_rotate_right(tree, curr_node);
				}

					/* Color the parent black and the grandparent red */
				curr_node->parent->color = Black;
				grandparent->color = Red;

					/* Left-rotate the grandparent's sub-tree */
				rbtree_rotate_left(tree, grandparent);
			}
		}
	}

	  /* Make sure that the root is black */
	tree->root->color = Black;
}

void rbtree_remove_fixup(Red_black_tree * tree, Red_black_node * node)
{
	Red_black_node * curr_node = node;
	Red_black_node * sibling;

	while (curr_node != tree->root && curr_node->color == Black) {
	  /* Get a pointer to the current node's sibling (notice that the node's 
	   * parent must exist, since the node is not the root).
	   */
		if (curr_node == curr_node->parent->left) {
		  /* If the current node is a left child, its sibling is the right 
		   * child of the parent.
		   */
			sibling = curr_node->parent->right;
      
			/* Check the sibling's color. Notice that NULL nodes are treated
			 * as if they are colored black.
			 */
			if (sibling && sibling->color == Red) {
		  /* In case the sibling is red, color it black and rotate.
			   * Then color the parent red (and the grandparent is now black).
			   */
				sibling->color = Black;
				curr_node->parent->color = Red;
				rbtree_rotate_left(tree, curr_node->parent);
				sibling = curr_node->parent->right;
			}
      
			if (sibling && 
			(!(sibling->left) || sibling->left->color == Black) && 
			(!(sibling->right) || sibling->right->color == Black))
			{
		  /* If the sibling has two black children, color it red */
				sibling->color = Red;
				if (curr_node->parent->color == Red) {
				  /* If the parent is red, we can safely color it black and terminate
					   * the fix-up process.
					   */
					curr_node->parent->color = Black;
					curr_node = tree->root;      /* In order to stop the while loop */
				}
				else {
				  /* The black depth of the entire sub-tree rooted at the parent is 
					   * now too small - fix it up recursively.
					   */
					curr_node = curr_node->parent;
				}
			}
			else {
				if (!sibling) {
				  /* Take special care of the case of a NULL sibling */
					if (curr_node->parent->color == Red) {
						curr_node->parent->color = Black;
						curr_node = tree->root;    /* In order to stop the while loop */
					}
					else {
						curr_node = curr_node->parent;
					}
				}
				else {
				  /* In this case, at least one of the sibling's children is red. 
					   * It is therfore obvious that the sibling itself is black.
					   */
					if (sibling->right && sibling->right->color == Red) {
					  /* If the right child of the sibling is red, color it black and
						   * rotate around the current parent.
						   */
						sibling->right->color = Black;
						rbtree_rotate_left(tree, curr_node->parent);
					}
					else {
					  /* If the left child of the sibling is red, rotate around the 
						   * sibling, then rotate around the new sibling of our current
						   * node.
						   */
						rbtree_rotate_right(tree, sibling);
						sibling = curr_node->parent->right;
						rbtree_rotate_left(tree, sibling);
					}
	  
					/* It is now safe to color the parent black and to terminate the 
						 * fix-up process.
						 */
					if (curr_node->parent->parent)
						curr_node->parent->parent->color = curr_node->parent->color;
					curr_node->parent->color = Black;
					curr_node = tree->root;      /* In order to stop the while loop */
				}
			}
		}
		else {
		  /* If the current node is a right child, its sibling is the left 
		   * child of the parent.
		   */
			sibling = curr_node->parent->left;

			      /* Check the sibling's color. Notice that NULL nodes are treated
			       * as if they are colored black.
			       */
			if (sibling && sibling->color == Red) {
		  /* In case the sibling is red, color it black and rotate.
			   * Then color the parent red (and the grandparent is now black).
			   */
				sibling->color = Black;
				curr_node->parent->color = Red;
				rbtree_rotate_right(tree, curr_node->parent);

				sibling = curr_node->parent->left;
			}

			if (sibling &&
			(!(sibling->left) || sibling->left->color == Black) && 
			(!(sibling->right) || sibling->right->color == Black))
			{
		  /* If the sibling has two black children, color it red */
				sibling->color = Red;
				if (curr_node->parent->color == Red) {
				  /* If the parent is red, we can safely color it black and terminate
					   * the fix-up process.
					   */
					curr_node->parent->color = Black;
					curr_node = tree->root;      /* In order to stop the while loop */
				}
				else {
				  /* The black depth of the entire sub-tree rooted at the parent is 
					   * now too small - fix it up recursively.
					   */
					curr_node = curr_node->parent;
				}
			}
			else {
				if (!sibling) {
				  /* Take special care of the case of a NULL sibling */
					if (curr_node->parent->color == Red) {
						curr_node->parent->color = Black;
						curr_node = tree->root;    /* In order to stop the while loop */
					}
					else {
						curr_node = curr_node->parent;
					}
				}
				else {
				  /* In this case, at least one of the sibling's children is red. 
					   * It is therfore obvious that the sibling itself is black.
					   */
					if (sibling->left && sibling->left->color == Red) {
					  /* If the left child of the sibling is red, color it black and
						   * rotate around the current parent
						   */
						sibling->left->color = Black;
						rbtree_rotate_right(tree, curr_node->parent);
					}
					else {
					  /* If the right child of the sibling is red, rotate around the 
						   * sibling, then rotate around the new sibling of our current 
						   * node
						   */
						rbtree_rotate_left(tree, sibling);
						sibling = curr_node->parent->left;
						rbtree_rotate_right(tree, sibling);
					}

						  /* It is now safe to color the parent black and to terminate the 
							   * fix-up process.
							   */
					if (curr_node->parent->parent)
						curr_node->parent->parent->color = curr_node->parent->color;
					curr_node->parent->color = Black;
					curr_node = tree->root;       /* In order to stop the while loop */
				}
			}
		}
	}

	  /* The root can always be colored black */
	curr_node->color = Black;
}

/* Traverse a red-black tree */
void rbtree_traverse(Red_black_tree * tree, Op * op)
{
	rbnode_traverse(tree->root, op);
}