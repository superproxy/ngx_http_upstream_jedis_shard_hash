#include <stdio.h>
#include "pointer.h"
node*  select_node2(node* ss[], char* key) {
	int size = sizeof(ss)/sizeof(ss[0]);
	for(int i=0;i<size;i++) {
		if(strcmp(ss[i]->name,key)==0) {
			return ss[i]; 
		}
	}
	return NULL;
}

