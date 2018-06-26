#include "UnixSocket.h"

UnixSocketServer::UnixSocketServer() : m_socketFd(0) {}

UnixSocketServer::~UnixSocketServer() {}

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

	return accept(m_socketFd, NULL, NULL);
}

int UnixSocketServer::writeSocket(int clientFd,void * buf, int len) {

	int ret = 0;

	do {
		if(NULL == buf) {
			ret = -1;
			break;
		}
		
		if (write(clientFd, buf, len) < 0) {
			printf("write error\n");
			ret = -1;
			break;
		}

	} while(0);

	return ret;
}

int UnixSocketServer::readSocket(int clientFd,void * buf, int len) {

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

int UnixSocketServer::closeSocket() {

	close(m_socketFd);

	return 0;
}

int UnixSocketServer::setSocketFdMethod(int clientFd,int method) {

	int flags = 0;
	int ret = 0;
	
	do {
		if(BLOCK == method) {
			flags = fcntl(clientFd, F_GETFL, 0); 
			fcntl(clientFd, F_SETFL, flags & ~O_NONBLOCK);
			break;
		}
		else if(NO_BLOCK == method) {
			flags = fcntl(clientFd, F_GETFL, 0); 
			fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
			break;		
		}
		else {
			ret = -1;
			break;
		}

	} while(0);
	
	return ret;
}

