#include <stdio.h>




union my_union { int i; short s; char c;};

void display(union my_union t) {
	printf("%d\n", t.i);
	printf("%d\n", t.c);
	printf("%d\n", t.s);
}

int main(int argc, char **argv) {
	union my_union test;
	int var0;
	test.i = 0;
	test.c = 'A';
	test.s = 16383;
	var0 = test.c;
	
	display(test);
	printf("%d\n", var0);
}
