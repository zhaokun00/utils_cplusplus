#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

void* tmain(void * arg) {

}

int main() {

	pthread_t tid;	 
	pthread_create ( & tid, NULL , tmain, NULL ) ;

	printf("Hello CMake!\n");
	return 0;
}

