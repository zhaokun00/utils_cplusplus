#include "UnixSocket.h"
#include <pthread.h>

#define PATH "/data/10.100.37.17/home/zhaok/test/utils_cplusplus/src/utils/unixSocket/test"

void * serverThreadProcess(void *obj) {

	int socketFd = 0;
	int clientFd = 0;
	
	socketFd = ipcOpen(PATH);
	ipcListen(socketFd, 60);
	clientFd = ipcAccept(socketFd);
	printf("server clinetFd = %d\n",clientFd); //与客户端的client值不同
	char buf = '1';
	
	while(1) {
		printf("write = %c\n",buf);
//		ipcWrite(clientFd,&buf,1);
		buf = buf + 1;
		sleep(1);
	}

	return NULL;
}

void * clientThreadProcess(void *clientThreadProcess) {

	int clientFd = 0;

	usleep(1000);
	
	clientFd = ipcConnect(PATH);

	printf("client clinetFd = %d\n",clientFd);
	char buf = 0;
	int ret = 0;

	sleep(1);
	
	while(1) {
		ret = ipcRead(clientFd, &buf, 1);
		printf("read = %c\n",buf);
		sleep(1);
	}

	return NULL;
}

int main() {

	int ret = 0;
	pthread_t serverThead;
	pthread_t clinetThead;
		
	pthread_create(&serverThead, NULL, serverThreadProcess, NULL);

	pthread_create(&clinetThead, NULL, clientThreadProcess, NULL);
	

	while(1) {
		sleep(1);
	}
	return ret;
}
