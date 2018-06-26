/*
 **************************************************************************************
 * Filename   : UnixSocket.h
 * Author 	  : zhaokun
 * Description: UnixSocket.h 该文件解决的是socket进程间通信
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef _UNIXSOCKET_H_
#define _UNIXSOCKET_H_

	
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BLOCK 	 0	//阻塞方式
#define NO_BLOCK 1 	//非阻塞方式

class UnixSocketClient {
	
	public:
		UnixSocketClient();
		~UnixSocketClient();
		int connectSocket(const char * pPathName);
		int writeSocket(void *buf, int len);
		int readSocket(void *buf, int len);
		int closeSocket();
		int setSocketFdMethod(int method);
		int getClientFd();
	private:
		int m_clientFd;
	
};

#endif

