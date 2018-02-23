#include "UnixSocket.h"

int ipcOpen(const char * pPathName) {

	int socketFd = -1;
   	struct sockaddr_un addr;
    int enable = 1;

	/*
		头文件：#include <unistd.h>
		
		定义函数：int unlink(const char * pathname);
		
		函数说明：unlink()会删除参数pathname 指定的文件. 如果该文件名为最后连接点, 但有其他进程打开了此文件, 则在所有关于此文件的文件描述词皆关闭后才会删除. 如果参数pathname 为一符号连接, 则此连接会被删除。
		
		返回值：成功则返回0, 失败返回-1, 错误原因存于errno
	*/
	unlink(pPathName);

   if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
	   memset(&addr, 0, sizeof(addr));
	   addr.sun_family = AF_UNIX;
	   strncpy(addr.sun_path, pPathName,sizeof(addr.sun_path)-1);

	   if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int)) < 0) {
		   printf("set resuse failed\n");
		   close(socketFd);
		   return -1;
	   }
	   
	   if (bind(socketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		   printf("bind failed \n");
		   close(socketFd);
		   return -1;
	   }
   }
   else {
	   printf("wrong socket:%d (%s)", socketFd, strerror(errno));
   }

   return socketFd;

}

int ipcListen(int socketFd, int number) {

    int ret = -1;
	
    do
    {
        ret = listen(socketFd, number);
        if (ret == -1) {
            perror("listen error");
            break;
        }
    }
    while (0);

    return ret;
}

int ipcAccept(int socketFd) {

    int clientFd = -1;

    do
    {
        clientFd = accept(socketFd, NULL, NULL);
    }
    while (0);

    return clientFd;
}

int ipcWrite(int clientFd, void *buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}
		
		if (write(clientFd, buf, len) < 0) {
			perror("write error\n");
			ret = -1;
			break;
		}	

	} while(0);
	
	return ret;

}

int ipcRead(int clientFd, void *buf, int len) {

    int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}

		ret = read(clientFd, buf, len); 
		
	} while(0);
    
    return ret;

}

int ipcServerClose(int socketFd,int clientFd) {

	int ret = 0;

	close(clientFd);
	ret = close(socketFd);

	return ret;
}

int ipcClientClose(int clinetFd) {
	
	return close(clinetFd);
}

int ipcConnect(const char * pPathName) {

	int clientFd = -1;
    struct sockaddr_un addr;
    int enable = 1;

    if ((clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
   
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, pPathName,sizeof(addr.sun_path) - 1);

//        fcntl(clientFd, F_SETFL, O_NONBLOCK); //设置非阻塞方式
//		ipcSetNoBlock(clientFd); //设置非阻塞方式
		ipcSetBlock(clientFd);
        if (setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int)) < 0)
        {
            printf("set resuse failed\n");
            close(clientFd);
            return -1;
        }
        
        if (connect(clientFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
        {
              perror("connect error\n");
              close(clientFd);
              return -EINVAL;
        }
    }

	return clientFd;
}

void ipcSetNoBlock(int fd) {

	int flags = fcntl(fd, F_GETFL, 0); 
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void ipcSetBlock(int fd) {

	int flags = fcntl(fd, F_GETFL, 0); 
	fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

