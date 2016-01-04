#include <stdio.h>


int main(void) {
	int a[] = {1,2,3,4};
	printf("&a:%d",&a);
	printf("\n");
	printf("a%d",a);
	printf("\n");
	printf("a[0]%d",a[0]);
	printf("\n");
	printf("&a[0]%d",&a[0]);
	printf("\n");
}
