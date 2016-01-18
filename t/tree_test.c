#include "base_test.h"

/*!
 */
int cmp_int(int a, int b)
{
	return (a > b) ? -1 : (a == b) ? 0 : 1;
}

/*!
 */
void my_print(int value)
{
	printf("%d ", value);
}


 
#ifdef   TREE_TEST
int main(int argc, char * argv [])   
#else
	int tree_test(int argc, char * argv []) 
#endif 
	{ 
	//int i, n;
	//
		//
	//
  //
	//if (argc < 2) {
		//fprintf(stderr, "ERROR: usage: rbtree number\n");
		//return -1;
	//}
	//n = atoi(argv[1]);
		
		Red_black_tree tree;	
		int n = 3;
	rbtree_init(&tree, (Comp*) cmp_int);

		int i;
	for (i = n - 1; i >= 0; --i)
		rbtree_insert(&tree, (void*) i);

	printf("n = %d, d = %d\n", n, rbtree_depth(&tree));

	rbtree_traverse(&tree, (Op*) my_print);
	printf("\n");
  
	rbtree_clean(&tree);
	return 0;
}