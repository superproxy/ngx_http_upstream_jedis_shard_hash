#ifndef _POINTER_H_
#define _POINTER_H_
#include <stdio.h>
typedef struct  {
	char * name;
	int age;
}node;

node*  select_node2(node* ss[], char* key); 
node*  select_node(node* ss[], char* key);
#endif
