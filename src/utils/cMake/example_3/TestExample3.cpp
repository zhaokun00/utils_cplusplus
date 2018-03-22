#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "hello.h"
#include "fun.h"

int main() {

	printf("Hello CMake!\n");
	hello();
	fun();
	return 0;
}

