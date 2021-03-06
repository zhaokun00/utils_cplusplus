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

	//使用socket通信时为双向通信,即服务端可以给客户端发送信息,客户端也可以给服务端发送信息
	
	//server端调用的函数
	int ipcOpen(const char * pPathName);
	int ipcListen(int socketFd, int number);
	int ipcAccept(int socketFd);
	int ipcWrite(int clientFd, void *buf, int len);
	int ipcRead(int clientFd, void *buf, int len);
	int ipcServerClose(int socketFd,int clientFd);

	//client端调用的函数
	int ipcConnect(const char * pPathName);
	int ipcClientClose(int clinetFd);
	
	void ipcSetNoBlock(int fd);
	void ipcSetBlock(int fd);

#define BLOCK 	 0	//阻塞方式
#define NO_BLOCK 1 	//非阻塞方式

class UnixSocketServer {

	public:
		UnixSocketServer();
		int openSocket(const char * pPathName,int listerNum);
		int acceptSocket();
		int writeSocket(void *buf, int len);
		int readSocket(void *buf, int len);
		int closeSocket();
		int setSocketFdMethod(int method);
		int getClientFd();

	private:
		int m_socketFd;
		int m_clientFd;
		
};

class UnixSocketClient {
	
	public:
		UnixSocketClient();
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

