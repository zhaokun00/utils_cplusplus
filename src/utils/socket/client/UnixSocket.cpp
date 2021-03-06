#include "UnixSocket.h"

UnixSocketClient::UnixSocketClient() : m_clientFd(0) {}

UnixSocketClient::~UnixSocketClient() {}

int UnixSocketClient::connectSocket(const char * pPathName) {

    struct sockaddr_un addr;
    int enable = 1;
	
	do {

		if(NULL == pPathName) {
			m_clientFd = -1;
			break;
		}

		if ((m_clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
	        memset(&addr, 0, sizeof(addr));
	        addr.sun_family = AF_UNIX;
	        strncpy(addr.sun_path, pPathName,sizeof(addr.sun_path) - 1);

			//默认为阻塞方式
	        if (setsockopt(m_clientFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int)) < 0) {
	            printf("set resuse failed\n");
	            close(m_clientFd);
				m_clientFd = -1;
	            break;
	        }
	        
	        if (connect(m_clientFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
	              perror("connect error\n");
	              close(m_clientFd);
	              m_clientFd = -1;
	           	  break;
	        }
		}		
	} while(0);

	return m_clientFd;
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


