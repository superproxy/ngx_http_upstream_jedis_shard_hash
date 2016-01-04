#include <stdio.h>
#include "pointer.h"

node*  select_node(node* ss[], char* key) {
	int size = sizeof(ss)/sizeof(ss[0]);
	for(int i=0;i<size;i++) {
		if(strcmp(ss[i]->name,key)==0) {
			return ss[i]; 
		}
	}
	return NULL;
}

char*  select(char* ss[], char* key) {
	return ss[0];
}
int test2(void) {
	char* ss[] = {"abc", "dfg", "hjk"};
	char* s = select(ss, "abc");
	printf("%s",s);
	node node1;
	node1.name="abc"; 
	node1.age=1;
	node* nodes[] = {&node1,&node1};
	node*  selected_node = select_node2(nodes, "abc");
	printf("node:{name:%s,age:%d}",selected_node->name, selected_node->age);
}
void test()
{
	char* ss[] = {"abc", "dfg", "hjk"};
	char* s = select(ss, "abc");
	printf("%s",s);
	node node1;
	node1.name="abc"; 
	node1.age=1;
	node* nodes[] = {&node1};
	node*  selected_node = select_node(nodes, "abc");
	printf("node:{name:%s,age:%d}",selected_node->name, selected_node->age);
}
int main(void) {
//   test();
   test2();
}
