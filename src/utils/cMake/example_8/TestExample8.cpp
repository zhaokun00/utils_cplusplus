#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

int main() {

	printf("Hello CMake!\n");

	#ifdef TEST1
		printf("define test1\n");
	#endif

	#if TEST2
		printf("TEST2 = 1\n");
	#else
		printf("TEST2 = 2\n");
	#endif
	
	return 0;
}

