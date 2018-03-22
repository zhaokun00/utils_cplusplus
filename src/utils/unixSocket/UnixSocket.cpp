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

	do {
		if(socketFd <= 0 || clientFd <= 0) {
			ret = -1;
			break;
		} 
		
		close(clientFd);
		ret = close(socketFd);

	} while(0);
	
	return ret;
}

int ipcClientClose(int clinetFd) {

	int ret = 0;

	do {
		if(clinetFd <= 0) {
			ret = -1;
		}

		ret = close(clinetFd);
	} while(0);
	
	return ret;
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


UnixSocketServer::UnixSocketServer() :
	m_socketFd(0),
	m_clientFd(0) {

}

int UnixSocketServer::openSocket(const char * pPathName, int listerNum) {

	int ret = 0;
	int socketFd = 0;
   	struct sockaddr_un addr;
    int enable = 1;
	
	do {
		if((NULL == pPathName) || (listerNum <= 0)) {
			ret = -1;
			break;
		}

		unlink(pPathName);

	    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
			m_socketFd = socketFd;
			memset(&addr, 0, sizeof(addr));
			addr.sun_family = AF_UNIX;
			strncpy(addr.sun_path, pPathName,sizeof(addr.sun_path)-1);

		    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int)) < 0) {
				close(socketFd);
				ret = -1;
		   		break;
		    }
		   
		    if (bind(socketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
				close(socketFd);
				ret = -1;
				break;
		   }

			if(listen(socketFd, listerNum) == -1) {
				close(socketFd);
				ret = -1;
				break;
			}
	   }
	   else {
		   printf("wrong socket:%d (%s)", socketFd, strerror(errno));
	   }
	} while(0);

	return ret;
}

int UnixSocketServer::acceptSocket() {

	m_clientFd = accept(m_socketFd, NULL, NULL);

	return m_clientFd;
}

int UnixSocketServer::writeSocket(void * buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}
		
		if (write(m_clientFd, buf, len) < 0) {
			perror("write error\n");
			ret = -1;
			break;
		}	

	} while(0);

	return ret;
}

int UnixSocketServer::readSocket(void * buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}

		ret = read(m_clientFd, buf, len); 
		
	} while(0);

	return ret;

}

int UnixSocketServer::closeSocket() {

	close(m_clientFd);
	close(m_socketFd);

	return 0;
}

int UnixSocketServer::setSocketFdMethod(int method) {

	int flags = 0;
	int ret = 0;
	
	do {
		if(BLOCK == method) {
			flags = fcntl(m_clientFd, F_GETFL, 0); 
			fcntl(m_clientFd, F_SETFL, flags & ~O_NONBLOCK);
			break;
		}
		else if(NO_BLOCK == method) {
			flags = fcntl(m_clientFd, F_GETFL, 0); 
			fcntl(m_clientFd, F_SETFL, flags | O_NONBLOCK);
			break;		
		}
		else {
			ret = -1;
			break;
		}

	} while(0);
	
	return ret;
}

int UnixSocketServer::getClientFd() {

	return m_clientFd;
}

UnixSocketClient::UnixSocketClient():
	m_clientFd(0) {

}

int UnixSocketClient::connectSocket(const char * pPathName) {

	int ret = 0;
	int clientFd = -1;
    struct sockaddr_un addr;
    int enable = 1;
	
	do {

		if(NULL == pPathName) {
			ret = -1;
			break;
		}

		if ((clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
	   		m_clientFd = clientFd;
	        memset(&addr, 0, sizeof(addr));
	        addr.sun_family = AF_UNIX;
	        strncpy(addr.sun_path, pPathName,sizeof(addr.sun_path) - 1);

			//默认为阻塞方式
	        if (setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int)) < 0) {
	            printf("set resuse failed\n");
	            close(clientFd);
				ret = -1;
	            break;
	        }
	        
	        if (connect(clientFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
	              perror("connect error\n");
	              close(clientFd);
	              ret = -1;
	           	  break;
	        }

			ret = m_clientFd;
		}		
	} while(0);

	return ret;
}

int UnixSocketClient::writeSocket(void * buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}
		
		if (write(m_clientFd, buf, len) < 0) {
			perror("write error\n");
			ret = -1;
			break;
		}	

	} while(0);

	return ret;

}

int UnixSocketClient::readSocket(void * buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}

		ret = read(m_clientFd, buf, len); 
		
	} while(0);

	return ret;
}

int UnixSocketClient::closeSocket() {

	return close(m_clientFd);
}

int UnixSocketClient::setSocketFdMethod(int method) {

	int flags = 0;
	int ret = 0;
	
	do {
		if(BLOCK == method) {
			flags = fcntl(m_clientFd, F_GETFL, 0); 
			fcntl(m_clientFd, F_SETFL, flags & ~O_NONBLOCK);
			break;
		}
		else if(NO_BLOCK == method) {
			flags = fcntl(m_clientFd, F_GETFL, 0); 
			fcntl(m_clientFd, F_SETFL, flags | O_NONBLOCK);
			break;		
		}
		else {
			ret = -1;
			break;
		}

	} while(0);
	
	return ret;
}

int UnixSocketClient::getClientFd() {

	return m_clientFd;
}


